#include <Adafruit_NeoPixel.h>
#include "NeopixelCtrlSplitTop.h"

#define NEOPIXEL_PIN 6
#define NUMPIXELS 80

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_PIN);        //need to create a pixel object FIRST
NeopixelCtrlSplitTop pixelsCtrl = NeopixelCtrlSplitTop(&pixels, NUMPIXELS, NEOPIXEL_PIN);     //then pass the pointer to the NeopixelCtrl object

unsigned long currentTime;

void setup() {
  pixels.begin();
  pixels.show();

  randomSeed(analogRead(A0));                                                 // for frenzy random colour

  pixelsCtrl.setLeftSegment(0, 19);
  // set the left segment pixels to 0 thru 19

  pixelsCtrl.setRightSegment(20, 39);
  // set the left segment pixels to 20 thru 39

  pixelsCtrl.setPlayerSegment(1, 42, 51);
  // set Player 1 segment pixels to 42 thru 51

  pixelsCtrl.setPlayerSegment(2, 54, 63);
  // set Player 2 segment pixels to 54 thru 63

  pixelsCtrl.setPlayerSegment(3, 66, 75);
  // set Player 3 segment pixels to 66 thru 75

  pixelsCtrl.setPlayerSegment(4, 78, 87);
  // set Player 4 segment pixels to 78 thru 87

  pixelsCtrl.setPlayerSegmentColour(1, 'R');
  // set Player 1 segment pixel colour to red

  pixelsCtrl.setPlayerSegmentColour(2, 'G');
  // set Player 2 segment pixel colour to green

  pixelsCtrl.setPlayerSegmentColour(3, 'B');
  // set Player 3 segment pixel colour to blue

  pixelsCtrl.setPlayerSegmentColour(4, 'Y');
  // set Player 4 segment pixel colour to yellow

  // (other colour options are cyan 'C' and magenta 'M')
}

void loop() {

  // This should show the left side as red (P1 colour), right side as green (P2 colour)
  // and countdown from both sides for 10 seconds

  currentTime = millis();
  
  //  pixelsCtrl.countDown(leftPlayer, rightPlayer, duration_in_seconds, startTime);
  pixelsCtrl.countDown(1, 2, 10, currentTime);    // Player 1 on the left half, Player 2 on the right half


  while (pixelsCtrl.isCountingDown()) {
    updatePixels(); // the millis function is already included
  }

  // Showing random colours for 10 seconds

  currentTime = millis();
  pixelsCtrl.frenzy(10, currentTime);

  while (pixelsCtrl.isFrenzy()) {
    updatePixels(); // the millis function is already included
  }

  // Counting up for 10 seconds using the entire top segment

  currentTime = millis();
  pixelsCtrl.countUp(10, currentTime);

  while (pixelsCtrl.isCountingUp()) {
    updatePixels(); // the millis function is already included
  }
}

void updatePixels() {

  // display speed calls are placed here so that the speed strip can change independently of the top strip control

  // change out random() to playerXXX.getSpeed() when ready
  // the randoms are for testing purpose

  pixelsCtrl.displaySpeed(1, random(0, 100));
  pixelsCtrl.displaySpeed(2, random(0, 100));
  pixelsCtrl.displaySpeed(3, random(0, 100));
  pixelsCtrl.displaySpeed(4, random(0, 100));

  // pixelsCtrl.displaySpeed(1, playerOne.getSpeed());
  // pixelsCtrl.displaySpeed(2, playerTwo.getSpeed());
  // pixelsCtrl.displaySpeed(3, playerThree.getSpeed());
  // pixelsCtrl.displaySpeed(4, playerFour.getSpeed());

  currentTime = millis();
  pixelsCtrl.updatePixelsColors(currentTime);
  pixels.show();
}
