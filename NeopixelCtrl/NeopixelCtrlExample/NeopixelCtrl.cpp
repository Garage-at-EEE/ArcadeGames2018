/*
  NeopixelCtrl.cpp - Library for Neopixel control in BishiBashi Game.
  Created by Karn Watcharasupat.
  Last Updated December 2018.
  Garage@EEE, School of Electrical and Electronic Engineering, Nanyang Technological University, Singapore.
*/

#define NEOPIXEL_DEBUG true

#include "NeopixelCtrl.h"

NeopixelCtrl :: NeopixelCtrl (Adafruit_NeoPixel* pixelsPtr, int totalLength, int pixelsPin) {
  _pixelsPtr = pixelsPtr;
  _totalLength = totalLength;
  _pixelsPin = pixelsPin;

  _isCountingDown = false;
  _isCountingUp = false;
  _isFrenzy = false;                 // initialize all actions to false

#if NEOPIXEL_DEBUG
  Serial.println("The NeopixelCtrl object is created.");
#endif
}

void NeopixelCtrl :: setTopSegment(int firstPixel, int lastPixel) {
#if NEOPIXEL_DEBUG
  Serial.println("Setting the top segment...");
#endif

  _topSegmentFirstIndex = firstPixel;
  _topSegmentLastIndex = lastPixel;

  _topSegmentLength = _topSegmentLastIndex - _topSegmentFirstIndex + 1;

  if (_topSegmentLength % 2 != 0) {
    _leftSegmentLength = _topSegmentLength / 2 + 1;
    _rightSegmentLength = _topSegmentLength - _leftSegmentLength;

#if NEOPIXEL_DEBUG
    Serial.println("Warning: The top segment has odd number of pixels!");
#endif
  }
  else {
    _leftSegmentLength = _topSegmentLength / 2;
    _rightSegmentLength = _leftSegmentLength;
#if NEOPIXEL_DEBUG
    Serial.println("The top segment has even number of pixels!");
#endif
  }                                               // in case of asymmetrical display
  // or if one pixel goes off the code can still sort of run with just minor index adjustment

  _rightSegmentFirstIndex = _topSegmentFirstIndex + _leftSegmentLength;

#if NEOPIXEL_DEBUG
  Serial.println("Done setting the top segment.");
#endif
}

void NeopixelCtrl :: setPlayerSegment(int playerCode, int firstPixel, int lastPixel) {
#if NEOPIXEL_DEBUG
  Serial.print("Setting Player "); Serial.print(playerCode); Serial.println("'s segment...");
#endif

  _playerPixelIndices[playerCode - 1][0] = firstPixel;
  _playerPixelIndices[playerCode - 1][1] = lastPixel;
  _playerPixelLengths[playerCode - 1] = lastPixel - firstPixel + 1;

#if NEOPIXEL_DEBUG
  Serial.println("Done.");
#endif
}

uint32_t NeopixelCtrl :: getColour(char colour) {
#if NEOPIXEL_DEBUG
  Serial.print("Getting colour...");
#endif
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
#if NEOPIXEL_DEBUG
  Serial.println("Done.");
#endif
}

void NeopixelCtrl :: setPlayerSegmentColour(int playerCode, char colour) {
#if NEOPIXEL_DEBUG
  Serial.print("Setting colour...");
#endif
  _playerColours[playerCode - 1] = getColour(colour);
}

void NeopixelCtrl :: countDown(int playerCode1, int playerCode2, int duration, unsigned long startTime) {
#if NEOPIXEL_DEBUG
  Serial.println("countDown() is called.");
#endif
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

#if NEOPIXEL_DEBUG
  Serial.print("Left player is Player "); Serial.println(_countdownPlayerLeft);
  Serial.print("Right player is Player "); Serial.println(_countdownPlayerRight);
#endif

  _countdownStartTime = startTime;
  _countdownDuration = duration * 1000UL; // convert milliseconds to seconds, UL for typecasting to unsigned long

#if NEOPIXEL_DEBUG
  Serial.print("Countdown start time is "); Serial.println(_countdownStartTime);
  Serial.print("Counting down for "); Serial.print(_countdownDuration); Serial.print("ms.");
#endif
}

void NeopixelCtrl :: updateCountDown(unsigned long currentTime) {

#if NEOPIXEL_DEBUG
  Serial.println("updateCountDown() is called.");
#endif

  unsigned long timeElapsed = currentTime - _countdownStartTime;

#if NEOPIXEL_DEBUG
  Serial.print(timeElapsed);
  Serial.println(" ms has elapsed.");
#endif

  if (timeElapsed > _countdownDuration) {
    _isCountingDown = false;

#if NEOPIXEL_DEBUG
    Serial.println("Duration exceeded. Exiting the function.");
#endif

    return;
  }

  int numPixelLeft = map(timeElapsed, 0, _countdownDuration, _leftSegmentLength, 0);
  int numPixelRight = map(timeElapsed, 0, _countdownDuration, _rightSegmentLength, 0);

#if NEOPIXEL_DEBUG
  Serial.print("Showing "); Serial.print(numPixelLeft);
  Serial.println(" pixels on the left.");
  Serial.print("Showing "); Serial.print(numPixelRight);
  Serial.println(" pixels on the right.");
#endif

  int leftSegmentLastIndex = _rightSegmentFirstIndex - 1;

#if NEOPIXEL_DEBUG
  Serial.println("Setting left segment...");
#endif

  for (int i = 0; i < numPixelLeft; i++) {
    _pixelsPtr->setPixelColor(leftSegmentLastIndex - i, _playerColours[_countdownPlayerLeft - 1]);

#if NEOPIXEL_DEBUG
    Serial.print(i + 1); Serial.println("th pixel is set.");
#endif
  }

#if NEOPIXEL_DEBUG
  Serial.println("Setting right segment...");
#endif

  for (int i = 0; i < numPixelRight; i++) {
    _pixelsPtr->setPixelColor(_rightSegmentFirstIndex + i, _playerColours[_countdownPlayerRight - 1]);

#if NEOPIXEL_DEBUG
    Serial.print(i + 1); Serial.println("th pixel is set.");
#endif
  }
}

void NeopixelCtrl :: countUp(int duration, unsigned long startTime) {

#if NEOPIXEL_DEBUG
  Serial.println("countUp() is called.");
#endif

  if (_isCountingDown || _isFrenzy) {
    _isCountingDown = false;
    _isFrenzy = false;
  }

  _isCountingUp = true;

  _countupStartTime = startTime;
  _countupDuration = duration * 1000UL; // convert milliseconds to seconds, UL for typecasting to unsigned long

#if NEOPIXEL_DEBUG
  Serial.print("Countup start time is "); Serial.println(_countupStartTime);
  Serial.print("Counting up for "); Serial.print(_countupDuration); Serial.print("ms.");
#endif
}

void NeopixelCtrl :: updateCountUp(unsigned long currentTime) {

#if NEOPIXEL_DEBUG
  Serial.println("updateCountUp() is called.");
#endif

  unsigned long timeElapsed = currentTime - _countupStartTime;

#if NEOPIXEL_DEBUG
  Serial.print(timeElapsed);
  Serial.println(" ms has elapsed.");
#endif

  if (timeElapsed > _countupDuration) {

#if NEOPIXEL_DEBUG
    Serial.println("Duration exceeded. Exiting the function.");
#endif

    _isCountingUp = false;
    return;
  }

  int numPixel = map(timeElapsed, 0, _countupDuration, 0, _topSegmentLength);

#if NEOPIXEL_DEBUG
  Serial.print("Showing "); Serial.print(numPixel);
  Serial.println(" pixels on each side.");
#endif

  for (int i = 0; i < numPixel; i++) {
    _pixelsPtr->setPixelColor(_topSegmentFirstIndex + i, _pixelsPtr->Color(255, 255, 255));

#if NEOPIXEL_DEBUG
    Serial.print(i + 1); Serial.println("th pixel is set.");
#endif

  }

}

void NeopixelCtrl :: frenzy(int duration, unsigned long startTime) {

#if NEOPIXEL_DEBUG
  Serial.println("frenzy() is called.");
#endif

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

#if NEOPIXEL_DEBUG
  Serial.println("updateFrenzy() is called.");
#endif

  if ((currentTime - _frenzyOldTime > 100UL) && (currentTime - _frenzyStartTime < _frenzyDuration)) {    // for stability, UL for typecasting to unsigned long
    for (int i = 0; i < _topSegmentLength; i++) {
      _pixelsPtr->setPixelColor(_topSegmentFirstIndex + i, _pixelsPtr->Color(random(0, 255), random(0, 255), random(0, 255)));
#if NEOPIXEL_DEBUG
      Serial.print(i + 1); Serial.println("th pixel is set.");
#endif
    }
    _frenzyOldTime = currentTime;
  }
  else if (currentTime - _frenzyStartTime > _frenzyDuration) {
    //else if was used because if (currentTime - _frenzyOldTime < 100UL) then the function does nothing

    _isFrenzy = false;

    for (int i = 0; i < _topSegmentLength; i++) {
      _pixelsPtr->setPixelColor(_topSegmentFirstIndex + i, _pixelsPtr->Color(0, 0, 0));
    }

#if NEOPIXEL_DEBUG
    Serial.println("Duration exceeded. Exiting the function.");
#endif
  }
}

void NeopixelCtrl :: displaySpeed(int playerCode, int buttonSpeed) {
  buttonSpeed = constrain(buttonSpeed, 0, MAXSPEED);
  _playerSpeed[playerCode - 1] = buttonSpeed;

#if NEOPIXEL_DEBUG
  Serial.print("Adding Player "); Serial.print(playerCode);
  Serial.println(" 's speed into the holding array.");

  Serial.print("The speed is ");
  Serial.println(_playerSpeed[playerCode - 1]);
#endif
}

void NeopixelCtrl :: updateSpeed() {
  for (int i = 0; i < _numPlayer; i++) {

#if NEOPIXEL_DEBUG
    Serial.print("Updating Player "); Serial.print(i + 1);
    Serial.println("th speed...");
#endif

    int thisPlayerSpeed = _playerSpeed[i];
    int thisPlayerFirstIndex = _playerPixelIndices[i][0];
    int thisPlayerPixelLength = _playerPixelLengths[i];
    uint32_t thisPlayerColour = _playerColours[i];

    int numPixel = map(thisPlayerSpeed, 0, MAXSPEED, 0, thisPlayerPixelLength);

#if NEOPIXEL_DEBUG
    Serial.print("The speed is ");
    Serial.println(thisPlayerSpeed);
    Serial.print("Showing "); Serial.print(numPixel); Serial.println(" pixels.");
#endif

    for (int j = 0; j < numPixel; j++) {
      _pixelsPtr->setPixelColor(thisPlayerFirstIndex + j, thisPlayerColour);

#if NEOPIXEL_DEBUG
      Serial.print(j + 1); Serial.println("th pixel is set.");
#endif

    }
  }
}

void NeopixelCtrl :: updatePixelsColors(unsigned long currentTime) {

#if NEOPIXEL_DEBUG
  Serial.println("updatePixelsColors() is called.");
#endif

  for (int i = 0; i < _totalLength; i++) {
    _pixelsPtr->setPixelColor(i, _pixelsPtr->Color(0, 0, 0));   // "turn off" all neopixels
  }

#if NEOPIXEL_DEBUG
  Serial.println("Set all pixels to black (0, 0, 0).");
#endif

  updateSpeed();    // always activated so that players can check if the button is working

  if (_isCountingDown) {
#if NEOPIXEL_DEBUG
    Serial.println("updatePixelsColors: counting down.");
#endif
    updateCountDown(currentTime);
  }
  else if (_isCountingUp) {
#if NEOPIXEL_DEBUG
    Serial.println("updatePixelsColors: counting up.");
#endif
    updateCountUp(currentTime);
  }
  else if (_isFrenzy) {
#if NEOPIXEL_DEBUG
    Serial.println("updatePixelsColors: going frenzy.");
#endif
    updateFrenzy(currentTime);
  }

  _pixelsPtr->show();
#if NEOPIXEL_DEBUG
  Serial.println("Showing pixels.");
#endif
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

void NeopixelCtrl::setNumPlayer(int numPlayer) {
  _numPlayer = numPlayer;

#if NEOPIXEL_DEBUG
  Serial.print("Setting the game for "); Serial.print(_numPlayer);
  Serial.println(" players");
#endif
}
