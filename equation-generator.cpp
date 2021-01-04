#include "equation-generator.h"

EquationGenerator::EquationGenerator() {
  this->initialValue = 10;
  this->modificators[0] = '+';
  this->modificators[1] = '-';
  this->modificators[2] = '*';
}

String EquationGenerator::generate(int level) {
  int maximum = initialValue + level;
  int minimum = 5 + level;
  int additionBoost = random(100) <= 33;
  
  if (additionBoost) {
      maximum *= (level + 2);
  }
  
  int minimumFirst = minimum - 1;
  int minimumSecond = minimum + 1;
  int minimumThird = minimum;
  
  char firstModificator = this->randomModificator();
  char secondModificator = this->randomModificator();
  
  if (additionBoost) {
      firstModificator = this->randomAddSubModificator();
      secondModificator = this->randomAddSubModificator();
  }
  
  int firstNumber = this->randomInt(minimumFirst, maximum);
  int secondNumber = this->randomInt(minimumSecond, maximum);
  int thirdNumber = this->randomInt(minimumThird, maximum);

  return
    String(firstNumber) 
    + " "
    + firstModificator 
    + " "
    + String(secondNumber) 
    + " "
    + secondModificator 
    + " "
    + String(thirdNumber);
}

char EquationGenerator::randomModificator() {
  return this->modificators[random(MODIFICATORS_SIZE)];
}

char EquationGenerator::randomAddSubModificator() {
  return this->modificators[random(MODIFICATORS_SIZE - 1)];
}

int EquationGenerator::randomInt(int minimum, int maximum) {
  return random((minimum + maximum), max(minimum, 1));
}
