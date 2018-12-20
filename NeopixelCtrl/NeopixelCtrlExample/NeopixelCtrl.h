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
    void displaySpeed(int playerCode, int buttonSpeed);
    void frenzy(int duration, unsigned long startTime);

    bool isCountingDown();
    bool isCountingUp();
    bool isFrenzy();


  private:

    uint32_t getColour(char colour);

    void updateCountDown(unsigned long currentTime);
    void updateCountUp(unsigned long currentTime);
    void updateSpeed();
    void updateFrenzy(unsigned long currentTime);

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

    int _countdownPlayerLeft;
    int _countdownPlayerRight;
    unsigned long _countdownStartTime;
    int _countdownDuration;

    unsigned long _countupStartTime;
    int _countupDuration;

    unsigned long _frenzyOldTime;
    unsigned long _frenzyStartTime;
    int _frenzyDuration;

    bool _isCountingDown;
    bool _isFrenzy;
    bool _isCountingUp;

    Adafruit_NeoPixel pixelsPtr;
};

#endif

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
