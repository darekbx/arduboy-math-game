#include <Arduino.h>

struct ResultWrapper {
  bool result;
  int equationResult;
};

class EquationValidator {

  private:
    
    String getValue(String data, char separator, int index);
    bool isMultiplicator(char manipulator);
    bool isAddition(char manipulator);
    bool isSubtraction(char manipulator);

  public:
    EquationValidator();
    ResultWrapper validate(String equation, int result);
};
