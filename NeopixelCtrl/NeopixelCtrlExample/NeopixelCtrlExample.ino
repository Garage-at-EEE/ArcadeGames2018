#include <Adafruit_NeoPixel.h>
#include "NeopixelCtrl.h"

#define NEOPIXEL_PIN 6
#define NUMPIXELS 80

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_PIN);        //need to create a pixel object FIRST
NeopixelCtrl pixelsCtrl = NeopixelCtrl(&pixels, NUMPIXELS, NEOPIXEL_PIN);     //then pass the pointer to the NeopixelCtrl object

unsigned long currenttime = millis();

void setup() {
  pixels.begin();
  pixels.show();

  randomSeed(analogRead(A0));                                                 // for frenzy

  pixelsCtrl.setTopSegment(0, 40);                                            // set the top segment first pixel index to 0
  pixelsCtrl.setPlayerSegments(4, 10, false, 0);                           // set 4 player segments of length 10 each. The index runs continuously between players
                                      // if the ENTIRE player segments need to be shifted to other index, indicate this as true
                                             // and set the first index of the player segments here
  pixelsCtrl.setPlayerSegmentsColour(1, 'R');
  pixelsCtrl.setPlayerSegmentsColour(2, 'G');
  pixelsCtrl.setPlayerSegmentsColour(3, 'B');
  pixelsCtrl.setPlayerSegmentsColour(4, 'Y');
}

void loop() {

  pixelsCtrl.countDown(1, 2, 10, currenttime);
  updatePixels();

  pixelsCtrl.frenzy(10, currenttime);
  updatePixels();

  pixelsCtrl.countUp(10, currenttime);
  updatePixels();
}

void updatePixels() {
  
  
  pixelsCtrl.updatePixelsColors(currenttime);
  pixels.show();
}
