/*
  NeopixelCtrlSplitTopSplitTop.cpp - Library for Neopixel control in BishiBashi Game.
  Created by Karn Watcharasupat.
  Last Updated December 2018.
  Garage@EEE, School of Electrical and Electronic Engineering, Nanyang Technological University, Singapore.
*/

#include "NeopixelCtrlSplitTop.h"

NeopixelCtrlSplitTop :: NeopixelCtrlSplitTop (Adafruit_NeoPixel* pixelsPtr, int totalLength, int pixelsPin) {
  _pixelsPtr = pixelsPtr;
  _totalLength = totalLength;
  _pixelsPin = pixelsPin;

  _isCountingDown = false;
  _isCountingUp = false;
  _isFrenzy = false;                 // initialize all actions to false
}

void NeopixelCtrlSplitTop :: setLeftSegment(int firstPixel, int lastPixel) {
  _leftSegmentFirstIndex = firstPixel;
  _leftSegmentLastIndex = lastPixel;

  _leftSegmentLength = _leftSegmentLastIndex - _leftSegmentFirstIndex + 1;
}

void NeopixelCtrlSplitTop :: setRightSegment(int firstPixel, int lastPixel) {
  _rightSegmentFirstIndex = firstPixel;
  _rightSegmentLastIndex = lastPixel;

  _rightSegmentLength = _rightSegmentLastIndex - _rightSegmentFirstIndex + 1;
}

void NeopixelCtrlSplitTop :: setPlayerSegment(int playerCode, int firstPixel, int lastPixel) {
  _playerPixelIndices[playerCode - 1][0] = firstPixel;
  _playerPixelIndices[playerCode - 1][1] = lastPixel;
  _playerPixelLengths[playerCode - 1] = lastPixel - firstPixel + 1;
}

uint32_t NeopixelCtrlSplitTop :: getColour(char colour) {
  switch (colour) {
    case 'R':
      return _pixelsPtr->Color(255, 0, 0);
    case 'G':
      return _pixelsPtr->Color(0, 255, 0);
    case 'B':
      return _pixelsPtr->Color(0, 0, 255);
    case 'Y':
      return _pixelsPtr->Color(255, 255, 0);
    case 'C':
      return _pixelsPtr->Color(0, 255, 255);
    case 'M':
      return _pixelsPtr->Color(255, 0, 255);
    default:
      return _pixelsPtr->Color(255, 255, 255);                      //return white if the colour input is undefined
  }
}

void NeopixelCtrlSplitTop :: setPlayerSegmentColour(int playerCode, char colour) {
  _playerColours[playerCode - 1] = getColour(colour);
}

void NeopixelCtrlSplitTop :: countDown(int playerCode1, int playerCode2, int duration, unsigned long startTime) {

  if (_isCountingUp || _isFrenzy) {
    _isCountingUp = false;
    _isFrenzy = false;
  }                                       // for safety

  _isCountingDown = true;

  if (playerCode1 > playerCode2) {
    int temp = playerCode1;
    playerCode2 = playerCode1;
    playerCode1 = temp;
  }                                       // for correct order of display

  _countdownPlayerLeft = playerCode1;
  _countdownPlayerRight = playerCode2;

  _countdownStartTime = startTime;
  _countdownDuration = duration * 1000UL; // convert seconds to milliseconds, UL for typecasting to unsigned long
}

void NeopixelCtrlSplitTop :: updateCountDown(unsigned long currentTime) {

  unsigned long timeElapsed = currentTime - _countdownStartTime;

  if (timeElapsed > _countdownDuration) {
    _isCountingDown = false;
    return;
  }

  int numPixelLeft = map(timeElapsed, 0, _countdownDuration, _leftSegmentLength, 0);
  int numPixelRight = map(timeElapsed, 0, _countdownDuration, _rightSegmentLength, 0);  // mapping from high to low (counting down)

  for (int i = 0; i < numPixelLeft; i++) {
    _pixelsPtr->setPixelColor(_leftSegmentLastIndex - i, _playerColours[_countdownPlayerLeft - 1]);  // displaying from center to edge
  }

  for (int i = 0; i < numPixelRight; i++) {
    _pixelsPtr->setPixelColor(_rightSegmentFirstIndex + i, _playerColours[_countdownPlayerRight - 1]); // displaying from center to edge
  }
}

void NeopixelCtrlSplitTop :: countUp(int duration, unsigned long startTime) {

  if (_isCountingDown || _isFrenzy) {
    _isCountingDown = false;
    _isFrenzy = false;
  }

  _isCountingUp = true;

  _countupStartTime = startTime;
  _countupDuration = duration * 1000UL; // convert seconds to milliseconds, UL for typecasting to unsigned long
}

void NeopixelCtrlSplitTop :: updateCountUp(unsigned long currentTime) {

  unsigned long timeElapsed = currentTime - _countupStartTime;

  if (timeElapsed > _countupDuration) {
    _isCountingUp = false;
    return;
  }

  int numPixelLeft = map(timeElapsed, 0, _countdownDuration, 0, _leftSegmentLength);
  int numPixelRight = map(timeElapsed, 0, _countdownDuration, 0, _rightSegmentLength);

  for (int i = 0; i < numPixelLeft; i++) {
    _pixelsPtr->setPixelColor(_leftSegmentLastIndex - i, _playerColours[_countdownPlayerLeft - 1]);
  }

  for (int i = 0; i < numPixelRight; i++) {
    _pixelsPtr->setPixelColor(_rightSegmentFirstIndex + i, _playerColours[_countdownPlayerRight - 1]);
  }

}

void NeopixelCtrlSplitTop :: frenzy(int duration, unsigned long startTime) {
  if (_isCountingDown || _isCountingUp) {
    _isCountingDown = false;
    _isCountingUp = false;
  }

  _isFrenzy = true;

  _frenzyDuration = duration * 1000UL; // convert second to millisecond, UL for typecasting to unsigned long
  _frenzyStartTime = startTime;
  _frenzyOldTime = startTime;
}

void NeopixelCtrlSplitTop :: updateFrenzy(unsigned long currentTime) {

  if ((currentTime - _frenzyOldTime > 100UL) && (currentTime - _frenzyStartTime < _frenzyDuration)) {    // for stability, UL for typecasting to unsigned long
    for (int i = 0; i < _leftSegmentLength; i++) {
      _pixelsPtr->setPixelColor(_leftSegmentFirstIndex + i, _pixelsPtr->Color(random(0, 255), random(0, 255), random(0, 255)));
    }
    for (int i = 0; i < _rightSegmentLength; i++) {
      _pixelsPtr->setPixelColor(_rightSegmentFirstIndex + i, _pixelsPtr->Color(random(0, 255), random(0, 255), random(0, 255)));
    }
  }
  else if (currentTime - _frenzyStartTime > _frenzyDuration) {

    //else if was used because if (currentTime - _frenzyOldTime < 100UL) then the function does nothing
    
    _isFrenzy = false;

    for (int i = 0; i < _leftSegmentLength; i++) {
      _pixelsPtr->setPixelColor(_leftSegmentFirstIndex + i, _pixelsPtr->Color(0, 0, 0));
    }
    for (int i = 0; i < _rightSegmentLength; i++) {
      _pixelsPtr->setPixelColor(_rightSegmentFirstIndex + i, _pixelsPtr->Color(0, 0, 0));
    }
  }

  _frenzyOldTime = currentTime;

}

void NeopixelCtrlSplitTop :: displaySpeed(int playerCode, int buttonSpeed) {
  buttonSpeed = constrain(buttonSpeed, 0, MAXSPEED);
  _playerSpeed[playerCode - 1] = buttonSpeed;
}

void NeopixelCtrlSplitTop :: updateSpeed() {
  for (int i = 0; i < _numPlayers; i++) {
    int thisPlayerSpeed = _playerSpeed[i];

    int thisPlayerFirstIndex = _playerPixelIndices[i][0];
    int thisPlayerPixelLength = _playerPixelLengths[i];

    int numPixel = map(thisPlayerSpeed, 0, MAXSPEED, 0, thisPlayerPixelLength);

    for (int j = 0; j < numPixel; j++) {
      _pixelsPtr->setPixelColor(thisPlayerFirstIndex + j, _playerColours[i]);
    }
  }
}

void NeopixelCtrlSplitTop :: updatePixelsColors(unsigned long currentTime) {

  for (int i = 0; i < _totalLength; i++) {
    _pixelsPtr->setPixelColor(i, _pixelsPtr->Color(0, 0, 0));   // "turn off" all neopixels
  }

  if (_isCountingDown) {
    updateCountDown(currentTime);
  }
  else if (_isCountingUp) {
    updateCountUp(currentTime);
  }
  else if (_isFrenzy) {
    updateFrenzy(currentTime);
  }

  updateSpeed();  // always activated so that players can check if the button is working

  _pixelsPtr->show();
}

bool NeopixelCtrlSplitTop :: isCountingDown() {
  return _isCountingDown;
}

bool NeopixelCtrlSplitTop :: isCountingUp() {
  return _isCountingUp;
}

bool NeopixelCtrlSplitTop :: isFrenzy() {
  return _isFrenzy;
}
