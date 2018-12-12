#include <Arduino.h>
#include "BishiBashiPlayer.h"

BishiBashiPlayer :: BishiBashiPlayer(int index, char colour) {
  _index = index;
  _colour = colour;
  _score = 0;
  _speed = 0;
}

void BishiBashiPlayer :: attach(Component component, int pin) {
  switch (component) {
    case LEFT_BUTTON:
#ifndef Button_h
      _leftButtonPin = pin;
      pinMode(_leftButtonPin, INPUT);
#else
      Button _leftButton = Button(pin);
#endif
      break;
    case RIGHT_BUTTON:
#ifndef Button_h
      _rightButtonPin = pin;
      pinMode(_rightButtonPin, INPUT);
#else
      Button _rightButton = Button(pin);;
#endif
      break;
    case LEFT_LED:
#ifndef LED_h
      _leftLEDPin = pin;
      pinMode(_leftLEDPin, OUTPUT);
#else
      LED _leftLED = LED(pin);
#endif
      break;
    case RIGHT_LED:
#ifndef LED_h
      _rightLEDPin = pin;
      pinMode(_rightLEDPin, OUTPUT);
#else
     LED _rightLED = LED(pin);
#endif
      break;
  }
}

unsigned int BishiBashiPlayer :: getSpeed() {
  return _speed;
}

unsigned int BishiBashiPlayer :: getScore() {
  return _score;
}

char BishiBashiPlayer :: getColour() {
  return _colour;
}
