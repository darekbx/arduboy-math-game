#include "equation-validator.h"

EquationValidator::EquationValidator() {
}

ResultWrapper EquationValidator::validate(String equation, int result) {
  struct ResultWrapper resultWrapper;
  const char seperator = ' ';
  int firstNumber = this->getValue(equation, seperator, 0).toInt();
  int secondNumber = this->getValue(equation, seperator, 2).toInt();
  int thirdNumber = this->getValue(equation, seperator, 4).toInt();
  
  char firstManipulator = this->getValue(equation, seperator, 1)[0];
  char secondManipulator = this->getValue(equation, seperator, 3)[0];
  
  if (this->isMultiplicator(firstManipulator) && this->isMultiplicator(secondManipulator)) {
      int equationResult = firstNumber * secondNumber * thirdNumber;
      resultWrapper.result = equationResult == result;
      resultWrapper.equationResult = equationResult;
      return resultWrapper;
  }
  
  if (this->isMultiplicator(firstManipulator)) {
      int firstValue = firstNumber * secondNumber;
  
      if (this->isAddition(secondManipulator)) {
          int equationResult = firstValue + thirdNumber;
          resultWrapper.result = equationResult == result;
          resultWrapper.equationResult = equationResult;
          return resultWrapper;
      } else if (this->isSubtraction(secondManipulator)) {
          int equationResult = firstValue - thirdNumber;
          resultWrapper.result = equationResult == result;
          resultWrapper.equationResult = equationResult;
          return resultWrapper;
      }
  }
  
  if (this->isMultiplicator(secondManipulator)) {
      int secondValue = secondNumber * thirdNumber;
  
      if (this->isAddition(firstManipulator)) {
          int equationResult = firstNumber + secondValue;
          resultWrapper.result = equationResult == result;
          resultWrapper.equationResult = equationResult;
          return resultWrapper;
      } else if (this->isSubtraction(firstManipulator)) {
          int equationResult = firstNumber - secondValue;
          resultWrapper.result = equationResult == result;
          resultWrapper.equationResult = equationResult;
          return resultWrapper;
      }
  }

  int firstValue;
  int equationResult;
  
  if (this->isAddition(firstManipulator)) {
    firstValue = firstNumber + secondNumber;
  } else {
    firstValue = firstNumber - secondNumber;
  }
  
  if (this->isAddition(secondManipulator)) {
    equationResult = firstValue + thirdNumber;
  } else {
    equationResult = firstValue - thirdNumber;
  }

  resultWrapper.result = equationResult == result;
  resultWrapper.equationResult = equationResult;
  return resultWrapper;
}

String EquationValidator::getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for (int i = 0; i <= maxIndex && found <= index; i++){
    if (data.charAt(i) == separator || i == maxIndex){
        found++;
        strIndex[0] = strIndex[1] + 1;
        strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

bool EquationValidator::isMultiplicator(char manipulator) {
  return manipulator == '*';
}

bool EquationValidator::isAddition(char manipulator) {
  return manipulator == '+';
}

bool EquationValidator::isSubtraction(char manipulator) {
  return manipulator == '-';
}
