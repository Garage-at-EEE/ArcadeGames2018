#include <Adafruit_NeoPixel.h>
#include "NeopixelCtrlSplitTop.h"

#define UPDATE_DELAY 100UL  // Put UL after the number to convert it to unsigned long
#define RGB_MAX 40 // If only Arduino Power is being used, do not go beyond 40
#define NEOPIXEL_DEBUG false

#define NEOPIXEL_PIN 2
#define NUMPIXELS 80

#define LEFT_SEGMENT_FIRST_PIXEL 0
#define LEFT_SEGMENT_LAST_PIXEL 9

#define RIGHT_SEGMENT_FIRST_PIXEL 13
#define RIGHT_SEGMENT_LAST_PIXEL 22

#define PLAYER1_FIRST_PIXEL 26
#define PLAYER1_LAST_PIXEL 35

#define PLAYER2_FIRST_PIXEL 39
#define PLAYER2_LAST_PIXEL 48

#define PLAYER3_FIRST_PIXEL 52
#define PLAYER3_LAST_PIXEL 61

#define PLAYER4_FIRST_PIXEL 65
#define PLAYER4_LAST_PIXEL 74

#define COUNTDOWN_DURATION 500
#define COUNTUP_DURATION 7500  
#define FRENZY_DURATION 500  

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_PIN);        //need to create a pixel object FIRST
NeopixelCtrlSplitTop pixelsCtrl = NeopixelCtrlSplitTop(&pixels, NUMPIXELS, NEOPIXEL_PIN);     //then pass the pointer to the NeopixelCtrl object

unsigned long currentTime;

void setup() {
  Serial.begin(9600);
  pixels.begin();
  pixels.show();

  randomSeed(analogRead(A0));                                                 // for frenzy random colour

  pixelsCtrl.setNumPlayer(4);

  pixelsCtrl.setLeftSegment(LEFT_SEGMENT_FIRST_PIXEL, LEFT_SEGMENT_LAST_PIXEL);
  pixelsCtrl.setRightSegment(RIGHT_SEGMENT_FIRST_PIXEL, RIGHT_SEGMENT_LAST_PIXEL);

  pixelsCtrl.setPlayerSegment(1, PLAYER1_FIRST_PIXEL, PLAYER1_LAST_PIXEL);
  pixelsCtrl.setPlayerSegment(2, PLAYER2_FIRST_PIXEL, PLAYER2_LAST_PIXEL);
  pixelsCtrl.setPlayerSegment(3, PLAYER3_FIRST_PIXEL, PLAYER3_LAST_PIXEL);
  pixelsCtrl.setPlayerSegment(4, PLAYER4_FIRST_PIXEL, PLAYER4_LAST_PIXEL);

  pixelsCtrl.setPlayerSegmentColour(1, 'R');
  // set Player 1 segment pixel colour to red 'R'

  pixelsCtrl.setPlayerSegmentColour(2, 'G');
  // set Player 2 segment pixel colour to green 'G'

  pixelsCtrl.setPlayerSegmentColour(3, 'B');
  // set Player 3 segment pixel colour to blue 'B'

  pixelsCtrl.setPlayerSegmentColour(4, 'Y');
  // set Player 4 segment pixel colour to yellow 'Y'

  // (other colour options are cyan 'C' and magenta 'M')
}

void loop() {

  // This should show the left side as red (P1 colour), right side as green (P2 colour)
  // and countdown from both sides for COUNTDOWN_DURATION seconds

  currentTime = millis();

  //  pixelsCtrl.countDown(leftPlayer, rightPlayer, duration_in_seconds, startTime);
  pixelsCtrl.countDown(1, 2, COUNTDOWN_DURATION, currentTime);    // Player 1 on the left half, Player 2 on the right half


  while (pixelsCtrl.isCountingDown()) {
    updatePixels(); // the millis function is already included
  }

  // Showing random colours for FRENZY_DURATION seconds

  currentTime = millis();
  pixelsCtrl.frenzy(FRENZY_DURATION, currentTime);

  while (pixelsCtrl.isFrenzy()) {
    updatePixels(); // the millis function is already included
  }

  // Counting up for COUNTUP_DURATION seconds using the entire top segment

  currentTime = millis();
  pixelsCtrl.countUp(COUNTUP_DURATION, currentTime);

  while (pixelsCtrl.isCountingUp()) {
    updatePixels(); // the millis function is already included
  }

}

void displayAllSpeeds() {

  // change out random() to playerXXX.getSpeed() when ready
  // the randoms are for testing purpose

  int speed1 = random(0, 100);
  int speed2 = random(0, 100);
  int speed3 = random(0, 100);
  int speed4 = random(0, 100);

  pixelsCtrl.displaySpeed(1, speed1);
  pixelsCtrl.displaySpeed(2, speed2);
  pixelsCtrl.displaySpeed(3, speed3);
  pixelsCtrl.displaySpeed(4, speed4);

  // pixelsCtrl.displaySpeed(1, playerOne.getSpeed());
  // pixelsCtrl.displaySpeed(2, playerTwo.getSpeed());
  // pixelsCtrl.displaySpeed(3, playerThree.getSpeed());
  // pixelsCtrl.displaySpeed(4, playerFour.getSpeed());
}

void updatePixels() {
  currentTime = millis();
  displayAllSpeeds();
  pixelsCtrl.updatePixelsColors(currentTime);
  pixels.show();
}
