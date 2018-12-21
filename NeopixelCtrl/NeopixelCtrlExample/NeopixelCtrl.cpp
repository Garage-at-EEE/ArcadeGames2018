/*
  NeopixelCtrl.cpp - Library for Neopixel control in BishiBashi Game.
  Created by Karn Watcharasupat.
  Last Updated 07 Dec 2018.
  Garage@EEE, School of Electrical and Electronic Engineering, Nanyang Technological University, Singapore.
*/

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "NeopixelCtrl.h"

#define MAXPLAYERS 4
#define MAXSPEED 100

int _pixelPin;
int _totalLength;

int _topSegmentFirstIndex;
int _topSegmentLastIndex;
int _topSegmentLength;
int _rightSegmentFirstIndex;
int _leftSegmentLength;
int _rightSegmentLength;

int _playerSegmentFirstIndex;
int _playerSegmentEachLength;
int _numPlayers;

int _playerCodes[MAXPLAYERS];
uint32_t _playerColours[MAXPLAYERS];
int _playerPixelIndices[MAXPLAYERS][2];
int _playerSpeed[MAXPLAYERS];
int _playerPixelLengths[MAXPLAYERS];

int _countdownPlayerLeft;
int _countdownPlayerRight;
unsigned long _countdownStartTime;
unsigned long _countdownDuration;

unsigned long _countupStartTime;
unsigned long _countupDuration;

unsigned long _frenzyOldTime;
unsigned long _frenzyStartTime;
unsigned long _frenzyDuration;

bool _isCountingDown;
bool _isFrenzy;
bool _isCountingUp;

Adafruit_NeoPixel* _pixelsPtr;

// methods

NeopixelCtrl :: NeopixelCtrl (Adafruit_NeoPixel* pixelsPtr, int totalLength, int pixelPin) {

  _pixelsPtr = pixelsPtr;
  _totalLength = totalLength;
  _pixelPin = pixelPin;

  _isCountingDown = false;
  _isCountingUp = false;
  _isFrenzy = false;                 // initialize all actions to false
}

void NeopixelCtrl :: setTopSegment(int firstPixel, int lastPixel) {
  _topSegmentFirstIndex = firstPixel;
  _topSegmentLastIndex = lastPixel;

  _topSegmentLength = _topSegmentLastIndex - _topSegmentFirstIndex + 1;

  if (_topSegmentLength % 2 != 0) {
    _leftSegmentLength = _topSegmentLength / 2 + 1;
    _rightSegmentLength = _topSegmentLength / 2;
  }
  else {
    _leftSegmentLength = _topSegmentLength / 2;
    _rightSegmentLength = _leftSegmentLength;
  }                                               // in case of asymmetrical display
  // or if one pixel goes off the code can still sort of run with just minor index adjustment

  _rightSegmentFirstIndex = _topSegmentFirstIndex + _leftSegmentLength;

  _playerSegmentFirstIndex = _rightSegmentFirstIndex + _rightSegmentLength;
  // set the player segment index to right after the last top segment index
  // can be overwritten in the setPlayerSegments function
}

void NeopixelCtrl :: setPlayerSegment(int playerCode, int firstPixel, int lastPixel) {
  _playerPixelIndices[playerCode - 1][0] = firstPixel;
  _playerPixelIndices[playerCode - 1][1] = lastPixel;
  _playerPixelLengths[playerCode - 1] = lastPixel - firstPixel + 1;
}

uint32_t NeopixelCtrl :: getColour(char colour) {
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
      return _pixelsPtr->Color(255, 255, 255);                      //return white if the colour input in undefined
  }
}

void NeopixelCtrl :: setPlayerSegmentColour(int playerCode, char colour) {
  _playerColours[playerCode - 1] = getColour(colour);
}

void NeopixelCtrl :: countDown(int playerCode1, int playerCode2, int duration, unsigned long startTime) {

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
  _countdownDuration = duration * 1000; // convert milliseconds to seconds
}

void NeopixelCtrl :: updateCountDown(unsigned long currentTime) {

  unsigned long timeElapsed = currentTime - _countdownStartTime;

  int numPixelLeft = map(timeElapsed, 0, _countdownDuration, _leftSegmentLength, 0);
  int numPixelRight = map(timeElapsed, 0, _countdownDuration, _rightSegmentLength, 0);

  for (int i = 0; i < numPixelLeft; i++) {
    _pixelsPtr->setPixelColor(_topSegmentFirstIndex + i, _playerColours[_countdownPlayerLeft - 1]);
  }

  for (int i = 0; i < numPixelRight; i++) {
    _pixelsPtr->setPixelColor(_rightSegmentFirstIndex + i, _playerColours[_countdownPlayerRight - 1]);
  }

  if (timeElapsed > _countdownDuration) {
    _isCountingDown = false;
  }

}

void NeopixelCtrl :: countUp(int duration, unsigned long startTime) {

  if (_isCountingDown || _isFrenzy) {
    _isCountingDown = false;
    _isFrenzy = false;
  }
  _isCountingUp = true;

  _countupStartTime = startTime;
  _countupDuration = duration;
}

void NeopixelCtrl :: updateCountUp(unsigned long currentTime) {

  unsigned long timeElapsed = currentTime - _countupStartTime;

  int numPixel = map(timeElapsed, 0, _countupDuration, 0, _topSegmentLength);

  for (int i = 0; i < numPixel; i++) {
    _pixelsPtr->setPixelColor(_topSegmentFirstIndex + i, _pixelsPtr->Color(255, 255, 255));
  }

  if (timeElapsed > _countupDuration) {
    _isCountingUp = false;
  }
}

void NeopixelCtrl :: displaySpeed(int playerCode, int buttonSpeed) {
  _playerSpeed[playerCode - 1] = buttonSpeed;
}

void NeopixelCtrl :: updateSpeed() {
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

void NeopixelCtrl :: updatePixelsColors(unsigned long currentTime) {

  for (int i = 0; i < _totalLength; i++) {
    _pixelsPtr->setPixelColor(i, _pixelsPtr->Color(0, 0, 0));
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

void NeopixelCtrl :: frenzy(int duration, unsigned long startTime) {
  if (_isCountingDown || _isCountingUp) {
    _isCountingDown = false;
    _isCountingUp = false;
  }
  _isFrenzy = true;

  _frenzyDuration = duration * 1000; // convert second to millisecond
  _frenzyStartTime = startTime;
}

void NeopixelCtrl :: updateFrenzy(unsigned long currentTime) {

  if ((currentTime - _frenzyOldTime > 50) && (currentTime - _frenzyOldTime < _frenzyDuration)) {    // for stability
    // arbitrary gap in milliseconds
    // can be adjusted

    for (int i = 0; i < _topSegmentLength; i++) {
      _pixelsPtr->setPixelColor(_topSegmentFirstIndex + i, _pixelsPtr->Color(random(0, 255), random(0, 255), random(0, 255)));
    }

  }
  else if (currentTime - _frenzyOldTime > _frenzyDuration) {
    _isFrenzy = false;

    for (int i = 0; i < _topSegmentLength; i++) {
      _pixelsPtr->setPixelColor(_topSegmentFirstIndex + i, _pixelsPtr->Color(0, 0, 0));
    }
  }

}

bool NeopixelCtrl :: isCountingDown() {
  return _isCountingDown;
}

bool NeopixelCtrl :: isCountingUp() {
  return _isCountingUp;
}

bool NeopixelCtrl :: isFrenzy() {
  return _isFrenzy;
}
