/*
  NeopixelCtrl.cpp - Library for Neopixel control in BishiBashi Game.
  Created by Karn Watcharasupat.
  Last Updated December 2018.
  Garage@EEE, School of Electrical and Electronic Engineering, Nanyang Technological University, Singapore.
*/

#include "NeopixelCtrl.h"

NeopixelCtrl :: NeopixelCtrl (Adafruit_NeoPixel* pixelsPtr, int totalLength, int pixelsPin) {
  _pixelsPtr = pixelsPtr;
  _totalLength = totalLength;
  _pixelsPin = pixelsPin;

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
    _rightSegmentLength = _topSegmentLength - _leftSegmentLength;
  }
  else {
    _leftSegmentLength = _topSegmentLength / 2;
    _rightSegmentLength = _leftSegmentLength;
  }                                               // in case of asymmetrical display
  // or if one pixel goes off the code can still sort of run with just minor index adjustment

  _rightSegmentFirstIndex = _topSegmentFirstIndex + _leftSegmentLength;
}

void NeopixelCtrl :: setPlayerSegment(int playerCode, int firstPixel, int lastPixel) {
  _playerPixelIndices[playerCode - 1][0] = firstPixel;
  Serial.println("Set first  pixel");
  _playerPixelIndices[playerCode - 1][1] = lastPixel;
  Serial.println("Set last pixel");
  _playerPixelLengths[playerCode - 1] = lastPixel - firstPixel + 1;
  Serial.println("Set length");
}

uint32_t NeopixelCtrl :: getColour(char colour) {
  Serial.println("Getting colour");
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
  Serial.println("Setting Colour");
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
  _countdownDuration = duration * 1000UL; // convert milliseconds to seconds, UL for typecasting to unsigned long
}

void NeopixelCtrl :: updateCountDown(unsigned long currentTime) {

  unsigned long timeElapsed = currentTime - _countdownStartTime;

  if (timeElapsed > _countdownDuration) {
    _isCountingDown = false;
    return;
  }

  int numPixelLeft = map(timeElapsed, 0, _countdownDuration, _leftSegmentLength, 0);
  int numPixelRight = map(timeElapsed, 0, _countdownDuration, _rightSegmentLength, 0);

  int leftSegmentLastIndex = _rightSegmentFirstIndex - 1;

  for (int i = 0; i < numPixelLeft; i++) {
    _pixelsPtr->setPixelColor(leftSegmentLastIndex - i, _playerColours[_countdownPlayerLeft - 1]);
  }

  for (int i = 0; i < numPixelRight; i++) {
    _pixelsPtr->setPixelColor(_rightSegmentFirstIndex + i, _playerColours[_countdownPlayerRight - 1]);
  }
}

void NeopixelCtrl :: countUp(int duration, unsigned long startTime) {

  if (_isCountingDown || _isFrenzy) {
    _isCountingDown = false;
    _isFrenzy = false;
  }

  _isCountingUp = true;

  _countupStartTime = startTime;
  _countupDuration = duration * 1000UL; // convert milliseconds to seconds, UL for typecasting to unsigned long
}

void NeopixelCtrl :: updateCountUp(unsigned long currentTime) {

  unsigned long timeElapsed = currentTime - _countupStartTime;

  if (timeElapsed > _countupDuration) {
    _isCountingUp = false;
    return;
  }

  int numPixel = map(timeElapsed, 0, _countupDuration, 0, _topSegmentLength);

  for (int i = 0; i < numPixel; i++) {
    _pixelsPtr->setPixelColor(_topSegmentFirstIndex + i, _pixelsPtr->Color(255, 255, 255));
  }

}

void NeopixelCtrl :: frenzy(int duration, unsigned long startTime) {
  if (_isCountingDown || _isCountingUp) {
    _isCountingDown = false;
    _isCountingUp = false;
  }
  
  _isFrenzy = true;

  _frenzyDuration = duration * 1000UL; // convert second to millisecond, UL for typecasting to unsigned long
  _frenzyStartTime = startTime;
  _frenzyOldTime = startTime;
}

void NeopixelCtrl :: updateFrenzy(unsigned long currentTime) {

  if ((currentTime - _frenzyOldTime > 100UL) && (currentTime - _frenzyStartTime < _frenzyDuration)) {    // for stability, UL for typecasting to unsigned long
    for (int i = 0; i < _topSegmentLength; i++) {
      _pixelsPtr->setPixelColor(_topSegmentFirstIndex + i, _pixelsPtr->Color(random(0, 255), random(0, 255), random(0, 255)));
    }
     _frenzyOldTime = currentTime;
  }
  else if (currentTime - _frenzyStartTime > _frenzyDuration) {  
    //else if was used because if (currentTime - _frenzyOldTime < 100UL) then the function does nothing
    
    _isFrenzy = false;

    for (int i = 0; i < _topSegmentLength; i++) {
      _pixelsPtr->setPixelColor(_topSegmentFirstIndex + i, _pixelsPtr->Color(0, 0, 0));
    }
  }

 

}

void NeopixelCtrl :: displaySpeed(int playerCode, int buttonSpeed) {
  buttonSpeed = constrain(buttonSpeed, 0, MAXSPEED);
  _playerSpeed[playerCode - 1] = buttonSpeed;
  Serial.println("Adding speed into array");
}

void NeopixelCtrl :: updateSpeed() {
  for (int i = 0; i < _numPlayers; i++) {
    int thisPlayerSpeed = _playerSpeed[i];
    
    int thisPlayerFirstIndex = _playerPixelIndices[i][0];
    int thisPlayerPixelLength = _playerPixelLengths[i];
    uint32_t thisPlayerColour = _playerColours[i];

    int numPixel = map(thisPlayerSpeed, 0, MAXSPEED, 0, thisPlayerPixelLength);
    Serial.print("numPixel =");
    Serial.println(numPixel);
    
    for (int j = 0; j < numPixel; j++) {
      _pixelsPtr->setPixelColor(thisPlayerFirstIndex + j, thisPlayerColour);
    
    Serial.print("j = ");
    Serial.println(j);
    }

    Serial.println("Setting player speed pixels");
  }
}

void NeopixelCtrl :: updatePixelsColors(unsigned long currentTime) {

  for (int i = 0; i < _totalLength; i++) {
    _pixelsPtr->setPixelColor(i, _pixelsPtr->Color(0, 0, 0));   // "turn off" all neopixels
  }
  
  updateSpeed();
  
  if (_isCountingDown) {
    updateCountDown(currentTime);
  }
  else if (_isCountingUp) {
    updateCountUp(currentTime);
  }
  else if (_isFrenzy) {
    updateFrenzy(currentTime);
  }

    // always activated so that players can check if the button is working

  _pixelsPtr->show();
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

void NeopixelCtrl::setNumPlayer(int numPlayer){
  _numPlayers = numPlayer;
  }
