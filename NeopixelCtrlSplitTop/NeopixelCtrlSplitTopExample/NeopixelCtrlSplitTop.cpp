/*
  NeopixelCtrlSplitTopSplitTop.cpp - Library for Neopixel control in BishiBashi Game.
  Created by Karn Watcharasupat.
  Last Updated December 2018.
  Garage@EEE, School of Electrical and Electronic Engineering, Nanyang Technological University, Singapore.
*/

#define NEOPIXEL_DEBUG true

#include "NeopixelCtrlSplitTop.h"

NeopixelCtrlSplitTop :: NeopixelCtrlSplitTop (Adafruit_NeoPixel* pixelsPtr, int totalLength, int pixelsPin) {
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

void NeopixelCtrlSplitTop :: setLeftSegment(int firstPixel, int lastPixel) {

#if NEOPIXEL_DEBUG
  Serial.print("Setting the left segment...");
#endif

  _leftSegmentFirstIndex = firstPixel;
  _leftSegmentLastIndex = lastPixel;

  _leftSegmentLength = _leftSegmentLastIndex - _leftSegmentFirstIndex + 1;
#if NEOPIXEL_DEBUG
  Serial.println("Done.");
#endif
}

void NeopixelCtrlSplitTop :: setRightSegment(int firstPixel, int lastPixel) {

#if NEOPIXEL_DEBUG
  Serial.print("Setting the right segment...");
#endif

  _rightSegmentFirstIndex = firstPixel;
  _rightSegmentLastIndex = lastPixel;

  _rightSegmentLength = _rightSegmentLastIndex - _rightSegmentFirstIndex + 1;
#if NEOPIXEL_DEBUG
  Serial.println("Done.");
#endif
}

void NeopixelCtrlSplitTop :: setPlayerSegment(int playerCode, int firstPixel, int lastPixel) {

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

uint32_t NeopixelCtrlSplitTop :: getColour(char colour) {
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
      return _pixelsPtr->Color(255, 255, 255);                      //return white if the colour input is undefined
  }
#if NEOPIXEL_DEBUG
  Serial.println("Done.");
#endif
}

void NeopixelCtrlSplitTop :: setPlayerSegmentColour(int playerCode, char colour) {
#if NEOPIXEL_DEBUG
  Serial.print("Setting colour...");
#endif
  _playerColours[playerCode - 1] = getColour(colour);
}

void NeopixelCtrlSplitTop :: countDown(int playerCode1, int playerCode2, int duration, unsigned long startTime) {
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
  Serial.print("Left player is Player"); Serial.println(_countdownPlayerLeft);
  Serial.print("Right player is Player"); Serial.println(_countdownPlayerRight);
#endif

  _countdownStartTime = startTime;
  _countdownDuration = duration * 1000UL; // convert seconds to milliseconds, UL for typecasting to unsigned long

#if NEOPIXEL_DEBUG
  Serial.print("Countdown start time is "); Serial.println(_countdownStartTime);
  Serial.print("Counting down for "); Serial.print(_countdownDuration); Serial.print("ms.");
#endif
}

void NeopixelCtrlSplitTop :: updateCountDown(unsigned long currentTime) {

#if NEOPIXEL_DEBUG
  Serial.println("updateCountDown() is called.");
#endif

  unsigned long timeElapsed = currentTime - _countdownStartTime;

#if NEOPIXEL_DEBUG
  Serial.print(timeElapsed);
  Serial.println(" ms has elapsed.");
#endif

  if (timeElapsed > _countdownDuration) {

#if NEOPIXEL_DEBUG
    Serial.println("Duration exceeded. Exiting the function.");
#endif

    _isCountingDown = false;
    return;
  }

  int numPixelLeft = map(timeElapsed, 0, _countdownDuration, _leftSegmentLength, 0);
  int numPixelRight = map(timeElapsed, 0, _countdownDuration, _rightSegmentLength, 0);  // mapping from high to low (counting down)

#if NEOPIXEL_DEBUG
  Serial.print("Showing "); Serial.print(numPixelLeft);
  Serial.println(" pixels on the left.");
  Serial.print("Showing "); Serial.print(numPixelRight);
  Serial.println(" pixels on the right.");
#endif

#if NEOPIXEL_DEBUG
  Serial.println("Setting left segment...");
#endif
  for (int i = 0; i < numPixelLeft; i++) {
    _pixelsPtr->setPixelColor(_leftSegmentLastIndex - i, _playerColours[_countdownPlayerLeft - 1]);  // displaying from center to edge

#if NEOPIXEL_DEBUG
    Serial.print(i + 1); Serial.println("th pixel is set.");
#endif

  }

#if NEOPIXEL_DEBUG
  Serial.println("Setting right segment...");
#endif

  for (int i = 0; i < numPixelRight; i++) {
    _pixelsPtr->setPixelColor(_rightSegmentFirstIndex + i, _playerColours[_countdownPlayerRight - 1]); // displaying from center to edge


#if NEOPIXEL_DEBUG
    Serial.print(i + 1); Serial.println("th pixel is set.");
#endif


  }
}

void NeopixelCtrlSplitTop :: countUp(int duration, unsigned long startTime) {

#if NEOPIXEL_DEBUG
  Serial.println("countUp() is called.");
#endif


  if (_isCountingDown || _isFrenzy) {
    _isCountingDown = false;
    _isFrenzy = false;
  }

  _isCountingUp = true;

  _countupStartTime = startTime;
  _countupDuration = duration * 1000UL; // convert seconds to milliseconds, UL for typecasting to unsigned long

#if NEOPIXEL_DEBUG
  Serial.print("Countup start time is "); Serial.println(_countupStartTime);
  Serial.print("Counting up for "); Serial.print(_countupDuration); Serial.print("ms.");
#endif
}

void NeopixelCtrlSplitTop :: updateCountUp(unsigned long currentTime) {

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

  int numPixelLeft = map(timeElapsed, 0, _countdownDuration, 0, _leftSegmentLength);
  int numPixelRight = map(timeElapsed, 0, _countdownDuration, 0, _rightSegmentLength);

#if NEOPIXEL_DEBUG
  Serial.print("Showing "); Serial.print(numPixelLeft);
  Serial.println(" pixels on the left.");
  Serial.print("Showing "); Serial.print(numPixelRight);
  Serial.println(" pixels on the right.");
#endif

#if NEOPIXEL_DEBUG
  Serial.println("Setting left segment...");
#endif

  for (int i = 0; i < numPixelLeft; i++) {
    _pixelsPtr->setPixelColor(_leftSegmentLastIndex - i, _playerColours[_countdownPlayerLeft - 1]);

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

void NeopixelCtrlSplitTop :: frenzy(int duration, unsigned long startTime) {

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


void NeopixelCtrlSplitTop :: updateFrenzy(unsigned long currentTime) {

#if NEOPIXEL_DEBUG
  Serial.println("updateFrenzy() is called.");
#endif

  if ((currentTime - _frenzyOldTime > 100UL) && (currentTime - _frenzyStartTime < _frenzyDuration)) {    // for stability, UL for typecasting to unsigned long
    for (int i = 0; i < _leftSegmentLength; i++) {
      _pixelsPtr->setPixelColor(_leftSegmentFirstIndex + i, _pixelsPtr->Color(random(0, 255), random(0, 255), random(0, 255)));
#if NEOPIXEL_DEBUG
      Serial.print(i + 1); Serial.println("th pixel on the left is set.");
#endif
    }
    for (int i = 0; i < _rightSegmentLength; i++) {
      _pixelsPtr->setPixelColor(_rightSegmentFirstIndex + i, _pixelsPtr->Color(random(0, 255), random(0, 255), random(0, 255)));
#if NEOPIXEL_DEBUG
      Serial.print(i + 1); Serial.println("th pixel on the right is set.");
#endif
    }
    _frenzyOldTime = currentTime;
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

#if NEOPIXEL_DEBUG
    Serial.println("Duration exceeded. Exiting the function.");
#endif
  }
}

void NeopixelCtrlSplitTop :: displaySpeed(int playerCode, int buttonSpeed) {
  buttonSpeed = constrain(buttonSpeed, 0, MAXSPEED);
  _playerSpeed[playerCode - 1] = buttonSpeed;

#if NEOPIXEL_DEBUG
  Serial.print("Adding Player "); Serial.print(playerCode);
  Serial.println(" 's speed into the holding array.");

  Serial.print("The speed is ");
  Serial.println(_playerSpeed[playerCode - 1]);
#endif
}

void NeopixelCtrlSplitTop :: updateSpeed() {
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

void NeopixelCtrlSplitTop :: updatePixelsColors(unsigned long currentTime) {

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

bool NeopixelCtrlSplitTop :: isCountingDown() {
  return _isCountingDown;
}

bool NeopixelCtrlSplitTop :: isCountingUp() {
  return _isCountingUp;
}

bool NeopixelCtrlSplitTop :: isFrenzy() {
  return _isFrenzy;
}

void NeopixelCtrlSplitTop :: setNumPlayer(int numPlayer) {
  _numPlayer = numPlayer;

#if NEOPIXEL_DEBUG
  Serial.print("Setting the game for "); Serial.print(_numPlayer);
  Serial.println(" players");
#endif
}
