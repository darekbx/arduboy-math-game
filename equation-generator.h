#include <Arduino.h>

#define MODIFICATORS_SIZE 3

class EquationGenerator {

  private:
    int initialValue;
    char modificators[MODIFICATORS_SIZE];
    
    char randomModificator();
    char randomAddSubModificator();
    int randomInt(int minimum, int maximum);

  public:
    EquationGenerator();
    String generate(int level);
};
