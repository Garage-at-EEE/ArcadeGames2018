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
int _topSegmentLength;
int _rightSegmentFirstIndex;
int _leftSegmentLength;
int _rightSegmentLength;

int _playerSegmentFirstIndex;
int _playerSegmentEachLength;
int _numPlayers;

int _playerCodes[MAXPLAYERS];
uint32_t _playerColours[MAXPLAYERS];
int _playerFirstIndex[MAXPLAYERS];
int _playerSpeed[MAXPLAYERS];

int _countdownPlayerLeft;
int _countdownPlayerRight;
unsigned long _countdownStartTime;
int _countdownDuration;

unsigned long _countupStartTime;
int _countupDuration;

unsigned long _frenzyOldTime;
unsigned long _frenzyStartTime;
int _frenzyDuration;

bool isCountingDown;
bool isFrenzy;
bool isCountingUp;

Adafruit_NeoPixel* _pixelsPtr;

NeopixelCtrl :: NeopixelCtrl (Adafruit_NeoPixel* pixelsPtr, int totalLength, int pixelPin) {

  _pixelsPtr = pixelsPtr;
  _totalLength = totalLength;
  _pixelPin = pixelPin;

  isCountingDown = false;
  isCountingUp = false;
  isFrenzy = false;
}

void NeopixelCtrl :: setTopSegment(int firstPixel, int segLength) {
  _topSegmentFirstIndex = firstPixel;
  _topSegmentLength = segLength;

  if (segLength % 2 != 0) {                                     
    _leftSegmentLength = segLength / 2 + 1;
    _rightSegmentLength = segLength / 2;
  }
  else {
    _leftSegmentLength = segLength / 2;
    _rightSegmentLength = _leftSegmentLength;
  }

  _rightSegmentFirstIndex = _topSegmentFirstIndex + _leftSegmentLength;
  _playerSegmentFirstIndex = _rightSegmentFirstIndex + _rightSegmentLength;
}

void NeopixelCtrl :: setPlayerSegments(int numPlayers, int segLength, bool gap, int startPixel) {

  if (gap) {
    _playerSegmentFirstIndex = startPixel;
  }

  for (int i = 0; i < numPlayers; i++) {
    _playerCodes[i] = i + 1;
  }

  for (int i = 0; i < numPlayers; i++) {
    _playerFirstIndex[i] = _playerSegmentFirstIndex + i * segLength;
  }
}

uint32_t getColour(char colour) {
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

void setPlayerSegmentsColour(int playerCode, char colour) {
  _playerColours[playerCode - 1] = getColour(colour);
}

void NeopixelCtrl :: countDown(int playerCode1, int playerCode2, int duration, unsigned long starttime) {

  if (isCountingUp || isFrenzy) {
    isCountingUp = false;
    isFrenzy = false;
  }
  isCountingDown = true;

  if (playerCode1 > playerCode2) {
    int temp = playerCode1;
    playerCode2 = playerCode1;
    playerCode1 = temp;
  }

  _countdownPlayerLeft = playerCode1;
  _countdownPlayerRight = playerCode2;

  _countdownStartTime = starttime;
  _countdownDuration = duration*1000;  // convert milliseconds to seconds
}

void NeopixelCtrl :: updateCountDown(unsigned long currenttime) {

  int timeElapsed = (int)(currenttime - _countdownStartTime);

  int numPixelLeft = map(timeElapsed, 0, _countdownDuration, _leftSegmentLength, 0);
  int numPixelRight = map(timeElapsed, 0, _countdownDuration, _rightSegmentLength, 0);

  for (int i = 0; i < numPixelLeft; i++) {
    _pixelsPtr->setPixelColor(_topSegmentFirstIndex + i, _playerColours[_countdownPlayerLeft - 1]);
  }

  for (int i = 0; i < numPixelRight; i++) {
    _pixelsPtr->setPixelColor(_rightSegmentFirstIndex + i, _playerColours[_countdownPlayerRight - 1]);
  }

}

void NeopixelCtrl :: countUp(int duration, unsigned long starttime) {

  if (isCountingDown || isFrenzy) {
    isCountingDown = false;
    isFrenzy = false;
  }
  isCountingUp = true;

  _countupStartTime = starttime;
  _countupDuration = duration;
}

void NeopixelCtrl :: updateCountUp(unsigned long currenttime) {

  int timeElapsed = (int)(currenttime - _countupStartTime);

  int numPixel = map(timeElapsed, 0, _countupDuration, 0, _topSegmentLength);

  int R = random(0, 255);
  int G = random(0, 255);
  int B = random(0, 255);

  for (int i = 0; i < numPixel; i++) {
    _pixelsPtr->setPixelColor(_topSegmentFirstIndex + i, _pixelsPtr->Color(R, G, B));
  }
}

void NeopixelCtrl :: displaySpeed(int playerCode, int buttonSpeed) {
  _playerSpeed[playerCode - 1] = buttonSpeed;
}

void NeopixelCtrl :: updateSpeed() {
  for (int i = 0; i < _numPlayers; i++) {
    int thisPlayerSpeed = _playerSpeed[i];
    int numPixel = map(thisPlayerSpeed, 0, 100, 0, _playerSegmentEachLength);

    for (int j = 0; j < numPixel; j++) {
      _pixelsPtr->setPixelColor(_playerSegmentFirstIndex + i * _playerSegmentEachLength + j, _playerColours[i]);
    }
  }
}

void NeopixelCtrl :: updatePixelsColors(unsigned long currenttime) {
  if (isCountingDown) {
    updateCountDown(currenttime);
  }
  else if (isCountingUp) {
    updateCountUp(currenttime);
  }
  else if (isFrenzy) {
    updateFrenzy(currenttime);
  }

  updateSpeed();  // always activated so that players can check if the button is working

  _pixelsPtr->show();
}

void NeopixelCtrl :: frenzy(int duration, unsigned long starttime) {
  if (isCountingDown || isCountingUp) {
    isCountingDown = false;
    isCountingUp = false;
  }
  isFrenzy = true;

  _frenzyDuration = duration * 1000;
  _frenzyStartTime = starttime;
}

void NeopixelCtrl :: updateFrenzy(unsigned long currenttime) {

  if ((currenttime - _frenzyOldTime > 50) && (currenttime - _frenzyOldTime < _frenzyDuration)) {    // for stability
    
    for (int i = 0; i < _topSegmentLength; i++) {
      _pixelsPtr->setPixelColor(_topSegmentFirstIndex + i, _pixelsPtr->Color(random(0, 255), random(0, 255), random(0, 255)));
    }

  }
  else if (currenttime - _frenzyOldTime > _frenzyDuration){
    isFrenzy = false;

    for (int i = 0; i < _topSegmentLength; i++) {
      _pixelsPtr->setPixelColor(_topSegmentFirstIndex + i, _pixelsPtr->Color(0,0,0));
    }
  }

}

/*

  getPixelPin();
  getTotalLength();

  getTopSegmentFirstIndex();
  getTopSegmentLastIndex();
  getTopSegmentLength();

  getLeftTopSegmentFirstIndex();
  getLeftTopSegmentLastIndex();
  getLeftTopSegmentLength();

  getRightTopSegmentFirstIndex();
  getRightTopSegmentLastIndex();
  getRightTopSegmentLength();

  getPlayerSegmentFirstIndex(int playerCode);
    getPlayerSegmentLength(int playerCode);
    getPlayerSegmentLastIndex(int playerCode);
*/

/*
  int _pixelPin;
  int _totalLength;

  int _topSegmentFirstIndex;
  int _topSegmentLength;
  int _rightSegmentFirstIndex;
  int _rightSegmentLength;

  int _playerSegmentFirstIndex;
  int _playerSegmentEachLength;
  int _numPlayers;
*/
