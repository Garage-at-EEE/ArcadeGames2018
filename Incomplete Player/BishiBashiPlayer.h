#include <Arduino.h>

enum Component {
  LEFT_BUTTON,
  RIGHT_BUTTON,
  LEFT_LED,
  RIGHT_LED,
};

class BishiBashiPlayer {
  public:
    BishiBashiPlayer(int index, char colour);
    void attach(Component component, int pin);
    unsigned int getSpeed();
    unsigned int getScore();
    char getColour();
  private:
    unsigned int _score;
    unsigned int _speed;
    char _colour;
    int _index;

#ifndef Button_h
    int _leftButtonPin;
    int _rightButtonPin;
#else
    Button _leftButton;
    Button _rightButton;
#endif

#ifndef LED_h
    int _leftLEDPin;
    int _rightLEDPin;
#else
    LED _leftLED;
    LED _rightLED;
#endif
};
