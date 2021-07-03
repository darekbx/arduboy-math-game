
#include <Arduboy2.h>
#include <EEPROM.h>
#include "equation-generator.h"
#include "equation-validator.h"

#include <avr/eeprom.h>

#define ERASE_MEMORY false
#define SERIAL_ENABLED false
#define SOLVED_COUNT_ADDRESS 10 
#define WRONG_COUNT_ADDRESS 12
#define SEED_ADDRESS 14

EquationGenerator equationGenerator;
EquationValidator equationValidator;
Arduboy2 arduboy;

const int minusIndex = 10;
const int backspaceIndex = 11;
int selectedNumber = 0;
int confirmNumber = -1;

int seed = 1;
int level = 1;
int solvedEquations = 0;
int correctAnswers = 0;
int wrongAnswers = 0;
String result = "";
String equation = "";
bool gameStarted = false;

void setup() {

  #if ERASE_MEMORY
    saveInt(SOLVED_COUNT_ADDRESS, 0);
    saveInt(WRONG_COUNT_ADDRESS, 0);
    saveInt(SEED_ADDRESS, 0);
    #endif

  #if SERIAL_ENABLED
    Serial.begin(9600);
    while (!Serial) { }
    Serial.println("Serial Ready");
    #endif
  
  arduboy.boot();
  arduboy.display();
  arduboy.flashlight();
  arduboy.waitNoButtons();
  arduboy.setFrameDuration(50);

  solvedEquations = readInt(SOLVED_COUNT_ADDRESS);
  wrongAnswers = readInt(WRONG_COUNT_ADDRESS);

  if (solvedEquations < 0) {
    solvedEquations = 0;
  }
  
  if (wrongAnswers < 0) {
    wrongAnswers = 0;
  }

  seed = readInt(SEED_ADDRESS);
  saveInt(SEED_ADDRESS, readInt(SEED_ADDRESS) + 1);
}

void loop() {

  if (!arduboy.nextFrame()) {
    return;
  }
  
  arduboy.clear();
  arduboy.setCursor(0, 0);
  arduboy.pollButtons();

  if (gameStarted) {

    handleButtons();
    
    displaySummary();
    displayLevel();
    displayNumberSelect();
    displayEquation(equation);
    displayResult(result);
  
    arduboy.digitalWriteRGB(RED_LED, RGB_OFF); 
    arduboy.digitalWriteRGB(GREEN_LED, RGB_OFF); 
  } else {
    arduboy.setCursor(4, 32);
    arduboy.print("Press B key to start");

    // TODO: remove
    arduboy.setCursor(0, 0);
    arduboy.print(seed);
    
    if (arduboy.justPressed(B_BUTTON)) {
      generateNewEquation();
      gameStarted = true;
    }
  }
  
  arduboy.display();
}

void displaySummary() {
  arduboy.setCursor(0, 0);
  arduboy.print(String(solvedEquations) + "/" + String(wrongAnswers));
}

void displayLevel() {
  arduboy.setCursor(80, 0);
  arduboy.print("Level: " + String(level));
}

void displayEquation(String equation) {
  const int CHAR_WIDTH = 6;
  int x = (WIDTH - (equation.length() * CHAR_WIDTH - 1)) / 2;

  arduboy.setTextSize(1);
  arduboy.setCursor(x, 12);
  arduboy.print(equation);
  arduboy.setTextSize(1);
}

void displayResult(String result) {
  const int CHAR_WIDTH = 11;
  int x = (WIDTH - (result.length() * CHAR_WIDTH - 1)) / 2;

  arduboy.setTextSize(2);
  arduboy.setCursor(x, 28);
  arduboy.print(result);
  arduboy.setTextSize(1);
}

void displayNumberSelect() {
  const int start = 6;
  const int digitWidth = 10;
  const int yPosition = 55;
  const int framePadding = 2;
  const int frameWidth = 9;
  const int frameHeight = 11;
  
  for (int i = 0; i <= 9; i++) {
    arduboy.setCursor(start + i * digitWidth, yPosition);
    arduboy.print(i);
  }

  // Draw minus
  arduboy.setCursor(start + minusIndex * digitWidth, yPosition);
  arduboy.print("-");
  
  // Draw backspace
  arduboy.setCursor(start + backspaceIndex * digitWidth, yPosition);
  arduboy.print("<");

  arduboy.drawRect(
    (start - framePadding) + selectedNumber * digitWidth,  yPosition - framePadding, 
    frameWidth, frameHeight, 
    WHITE);

  if (confirmNumber != -1) {
    arduboy.setCursor(start + selectedNumber * digitWidth, yPosition);
    arduboy.setTextColor(BLACK);
    arduboy.setTextBackground(WHITE);
    arduboy.print(confirmNumber); 
    arduboy.setTextBackground(BLACK);
    arduboy.setTextColor(WHITE);
  }

  const int yDelimiter = yPosition - framePadding * 2;
  arduboy.drawLine(0, yDelimiter, 128, yDelimiter, WHITE);
}

void generateNewEquation() {
  //arduboy.initRandomSeed();
  randomSeed(seed + millis());
  equation = equationGenerator.generate(level);
}

void handleButtons() {
  // Number select buttons
  if (arduboy.justPressed(RIGHT_BUTTON)) {
    confirmNumber = -1;
    selectedNumber = selectedNumber + 1;
    if (selectedNumber > 11) {
      selectedNumber = 0;
    }
  }
  
  if (arduboy.justPressed(LEFT_BUTTON)) {
    confirmNumber = -1;
    selectedNumber = selectedNumber - 1;
    if (selectedNumber < 0) {
      selectedNumber = 11;
    }
  }

  // Number / backspace confirm
  if (arduboy.justPressed(B_BUTTON)) {
    // 10 is a minus, 11 is a backspace
    if (selectedNumber == backspaceIndex) {
      // Remove last
      const int resultLength = result.length();
      if (resultLength > 0) {
        result.remove(resultLength - 1);
      }
    } else if (selectedNumber == minusIndex) {
      // Add minus
      result += "-";
    } else {
      confirmNumber = selectedNumber;
      result += String(selectedNumber);
    }

  }
  
  // Validate
  if (arduboy.justPressed(A_BUTTON)) {
    ResultWrapper resultWrapper = equationValidator.validate(equation, result.toInt());
    solvedEquations = solvedEquations + 1;
    
    #if SERIAL_ENABLED
      Serial.print("Equation: ");
      Serial.println(equation);
      Serial.print("User result: ");
      Serial.println(result.toInt());
      Serial.print("Validator result: ");
      Serial.println(resultWrapper.result);
      #endif

    if (resultWrapper.result) {
      arduboy.digitalWriteRGB(GREEN_LED, RGB_ON);
      correctAnswers = correctAnswers + 1;
      if (correctAnswers % 10 == 0) {
        level++;
      }
    } else {
      wrongAnswers = wrongAnswers + 1;
      arduboy.digitalWriteRGB(RED_LED, RGB_ON); 
    }

    saveInt(SOLVED_COUNT_ADDRESS, solvedEquations);
    saveInt(WRONG_COUNT_ADDRESS, wrongAnswers);
    
    delay(250);

    result = "";
    generateNewEquation();
  }
}

void saveInt(int address, int number) { 
  EEPROM.write(address, number >> 8);
  EEPROM.write(address + 1, number & 0xFF);
}

int readInt(int address) {
  return (EEPROM.read(address) << 8) + EEPROM.read(address + 1);
}
