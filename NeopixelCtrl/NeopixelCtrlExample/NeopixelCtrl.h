/*
  NeopixelCtrl.h - Header file for Neopixel control in BishyBashy Game.
  Created by Karn Watcharasupat.
  Last Updated 07 Dec 2018.
  Garage@EEE, School of Electrical and Electronic Engineering, Nanyang Technological University, Singapore.
*/

#define MAXPLAYERS 4
#define MAXSPEED 100

#ifndef NeopixelCtrl_h
#define NeopixelCtrl_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class NeopixelCtrl {
  public:
    NeopixelCtrl(Adafruit_NeoPixel* pixelsPtr, int pixelPin, int totalLength);

    void setTopSegment(int firstPixel, int segLength);
    
    void setPlayerSegment(int playerCode, int firstPixel, int lastPixel);
    void setPlayerSegmentColour(int playerCode, char colour);

    void updatePixelsColors(unsigned long currentTime);

    void countDown(int playerCode1, int playerCode2, int duration, unsigned long startTime);
    void countUp(int duration, unsigned long startTime);
    
    void frenzy(int duration, unsigned long startTime);

    void displaySpeed(int playerCode, int buttonSpeed);

    bool isCountingDown();
    bool isCountingUp();
    bool isFrenzy();

  private:

    uint32_t getColour(char colour);

    void updateCountDown(unsigned long currentTime);
    void updateCountUp(unsigned long currentTime);
    void updateSpeed(void);
    void updateFrenzy(unsigned long currentTime);

    int _pixelPin;
    int _totalLength;

    int _topSegmentFirstIndex;
    int _topSegmentLastIndex;
    int _topSegmentLength;
    int _rightSegmentFirstIndex;
    int _leftSegmentLength;
    int _rightSegmentLength;

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
};

#endif
