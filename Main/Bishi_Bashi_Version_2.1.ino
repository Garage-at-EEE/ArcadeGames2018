//INcludes
#include <Adafruit_NeoPixel.h>
#include <NeopixelCtrlSplitTop.h>
#include <BishiBashiPlayerQ.h>



//DEBUGGING

#define DEBUG true
#define DEBUG_STANDBY false
#define DEBUGSWITCH_1 false
#define DEBUGSWITCH_2 false
#define DEBUGSWITCH_3 false
#define DEBUGSWITCH_4 true
#define DEBUG_SEGMENT_SPEED_PRINT false


#if DEBUG && DEBUGSWITCH_1
#define DEBUG1 true
#else
#define DEBUG1 false
#endif


#if DEBUG && DEBUGSWITCH_2
#define DEBUG2 true
#else
#define DEBUG2 false
#endif


#if DEBUG && DEBUGSWITCH_1
#define DEBUG1 true
#else
#define DEBUG1 false
#endif


#if DEBUG && DEBUGSWITCH_3
#define DEBUG3 true
#else
#define DEBUG3 false
#endif


#if DEBUG && DEBUGSWITCH_4
#define DEBUG4 true
#else
#define DEBUG4 false
#endif
//Hardware Variables

#define BLUE_LEFT_BUTTON_PIN A3
#define BLUE_RIGHT_BUTTON_PIN A6
#define BLUE_LEFT_LED_PIN A4
#define BLUE_RIGHT_LED_PIN A5
#define BLUE_INDEX 1

#define GREEN_LEFT_BUTTON_PIN 7
#define GREEN_RIGHT_BUTTON_PIN 12
#define GREEN_LEFT_LED_PIN 5
#define GREEN_RIGHT_LED_PIN 10
#define GREEN_INDEX 2

#define RED_LEFT_BUTTON_PIN A0
#define RED_RIGHT_BUTTON_PIN A2
#define RED_LEFT_LED_PIN 13
#define RED_RIGHT_LED_PIN A1
#define RED_INDEX 3

#define YELLOW_LEFT_BUTTON_PIN 6
#define YELLOW_RIGHT_BUTTON_PIN 9
#define YELLOW_LEFT_LED_PIN 8
#define YELLOW_RIGHT_LED_PIN 4
#define YELLOW_INDEX 4


#define NEOPIXEL_PIN 11
#define NUMPIXELS 60


//Software definitions
#define SAMPLESIZE 1
#define SPEED_SAMPLE_DURATION 1000

#define REDPLAYER 'r'
#define YELLOWPLAYER 'y'
#define GREENPLAYER 'g'
#define BLUEPLAYER 'b'

//Gameplay Variables
#define TOTAL_SEGMENT_DURATION 8000
#define PRE_SEGMENT_DURATION 500

//For the creation of random array
#define PLAYER_COUNT        4
#define REPEAT_PER_PLAYER   3
#define ROUND_COUNT_VAR     (REPEAT_PER_PLAYER * PLAYER_COUNT / 2)
int playOrderActual[ROUND_COUNT_VAR][2];


BishiBashiPlayer playerRed(RED_INDEX, RED);
BishiBashiPlayer playerBlue(BLUE_INDEX, BLUE);
BishiBashiPlayer playerGreen(GREEN_INDEX, GREEN);
BishiBashiPlayer playerYellow(YELLOW_INDEX, YELLOW);

enum playingStatus {
  STANDBY,
  PLAYING

};


playingStatus currentPlayingStatus = STANDBY;
unsigned long standbyStartTime;
unsigned long currentTime;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_PIN);
//NeopixelCtrl pixelsCtrl = NeopixelCtrl(&pixels, NUMPIXELS, NEOPIXEL_PIN);

void setup() {
  playerRed.attach(LEFT_BUTTON, RED_LEFT_BUTTON_PIN, SAMPLESIZE);
  playerRed.attach(RIGHT_BUTTON, RED_RIGHT_BUTTON_PIN, SAMPLESIZE);
  playerRed.attach(LEFT_LED, RED_LEFT_LED_PIN, SAMPLESIZE);
  playerRed.attach(RIGHT_LED, RED_RIGHT_LED_PIN, SAMPLESIZE);

  playerGreen.attach(LEFT_BUTTON, GREEN_LEFT_BUTTON_PIN, SAMPLESIZE);
  playerGreen.attach(RIGHT_BUTTON, GREEN_RIGHT_BUTTON_PIN, SAMPLESIZE);
  playerGreen.attach(LEFT_LED, GREEN_LEFT_LED_PIN, SAMPLESIZE);
  playerGreen.attach(RIGHT_LED, GREEN_RIGHT_LED_PIN, SAMPLESIZE);

  playerBlue.attach(LEFT_BUTTON, BLUE_LEFT_BUTTON_PIN, SAMPLESIZE);
  playerBlue.attach(RIGHT_BUTTON, BLUE_RIGHT_BUTTON_PIN, SAMPLESIZE);
  playerBlue.attach(LEFT_LED, BLUE_LEFT_LED_PIN, SAMPLESIZE);
  playerBlue.attach(RIGHT_LED, BLUE_RIGHT_LED_PIN, SAMPLESIZE);

  playerYellow.attach(LEFT_BUTTON, YELLOW_LEFT_BUTTON_PIN, SAMPLESIZE);
  playerYellow.attach(RIGHT_BUTTON, YELLOW_RIGHT_BUTTON_PIN, SAMPLESIZE);
  playerYellow.attach(LEFT_LED, YELLOW_LEFT_LED_PIN, SAMPLESIZE);
  playerYellow.attach(RIGHT_LED, YELLOW_RIGHT_LED_PIN, SAMPLESIZE);

  pixels.begin();
  pixels.show();

  randomSeed(analogRead(A0));                                                 // for frenzy

//  pixelsCtrl.setTopSegment(0, 20);                                            // set the top segment first pixel index to 0
//  pixelsCtrl.setPlayerSegments(4, 10, false, 0);                           // set 4 player segments of length 10 each. The index runs continuously between players
  // if the ENTIRE player segments need to be shifted to other index, indicate this as true
  // and set the first index of the player segments here
//  pixelsCtrl.setPlayerSegmentsColour(1, 'R'); // red
  //pixelsCtrl.setPlayerSegmentsColour(2, 'G'); // green
  //pixelsCtrl.setPlayerSegmentsColour(3, 'B'); // blue
  //pixelsCtrl.setPlayerSegmentsColour(4, 'Y'); // yellow (other options are cyan 'C' and magenta 'M')
#if DEBUG
  Serial.begin(9600);
#endif
}



void loop() {
  // put your main code here, to run repeatedly:

#if DEBUG1
  Serial.print("Start of loop print. 0 = STANDBY, 1 = PLAYING:  ");
  Serial.println(currentPlayingStatus);
#endif
  if (currentPlayingStatus == STANDBY) {
    standbyProtocol();
  }
  if (currentPlayingStatus == PLAYING) {
    gameProtocol();
  }
  //gameProtocol();
  //segmentProtocol(1, 2, millis(), PRE_SEGMENT_DURATION, TOTAL_SEGMENT_DURATION);


}



void updatePlayingState(unsigned long currentTime, playingStatus newStatus, playingStatus *currentPlayingStatus, unsigned long* standbyStartTime) {
  if (newStatus == STANDBY && *currentPlayingStatus == PLAYING) {
    *standbyStartTime = currentTime;
  }
  *currentPlayingStatus = newStatus;
}








void standbyProtocol() {
#if DEBUG_STANDBY
  Serial.print("Red is ready?:");
  Serial.println(playerRed.playerIsReady());
  Serial.print("Green is ready?:");
  Serial.println(playerGreen.playerIsReady());
  Serial.print("Blue is ready?:");
  Serial.println(playerBlue.playerIsReady());
  Serial.print("Yellow is ready?:");
  Serial.println(playerYellow.playerIsReady());
#endif
  if (playerRed.playerIsReady() && playerBlue.playerIsReady() && playerGreen.playerIsReady() && playerYellow.playerIsReady()) {
#if DEBUG_STANDBY
    Serial.println("just switched to playing game");
#endif
    updatePlayingState(currentTime, PLAYING, &currentPlayingStatus, &standbyStartTime);

  }
  else {
#if DEBUG_STANDBY
    Serial.println("still on standby");
#endif

    playerRed.playerOnLed(LEFT);
    playerRed.playerOnLed(RIGHT);
    playerYellow.playerOnLed(LEFT);
    playerYellow.playerOnLed(RIGHT);
    playerGreen.playerOnLed(LEFT);
    playerGreen.playerOnLed(RIGHT);
    playerBlue.playerOnLed(LEFT);
    playerBlue.playerOnLed(RIGHT);

    if (playerRed.playerIsHeld(LEFT) ) {
      playerRed.playerOffLed(LEFT);
    }
    if (playerRed.playerIsHeld(RIGHT)) {
      playerRed.playerOffLed(RIGHT);
    }

    if (playerGreen.playerIsHeld(LEFT) ) {
      playerGreen.playerOffLed(LEFT);
    }
    if (playerGreen.playerIsHeld(RIGHT)) {
      playerGreen.playerOffLed(RIGHT);
    }

    if (playerBlue.playerIsHeld(LEFT) ) {
      playerBlue.playerOffLed(LEFT);
    }
    if (playerBlue.playerIsHeld(RIGHT)) {
      playerBlue.playerOffLed(RIGHT);
    }

    if (playerYellow.playerIsHeld(LEFT) ) {
      playerYellow.playerOffLed(LEFT);
    }
    if (playerYellow.playerIsHeld(RIGHT)) {
      playerYellow.playerOffLed(RIGHT);
    }

  }
}

void gameProtocol() {
  unsigned long currentGameTime = millis();
  RandomOrder();
#if DEBUG2
  Serial.println("GAME STARTS TIME:");
  Serial.println(currentGameTime);
  Serial.println("_________GAME PLAY ORDER___________");
  for (int i = 0; i < ROUND_COUNT_VAR; i++) {
    Serial.print(playOrderActual[i][0]);
    Serial.print(" VS ");
    Serial.print(playOrderActual[i][1]);
    Serial.print(",  ");
  }
  Serial.println("");
#endif
  for (int i = 0; i < ROUND_COUNT_VAR; i++) {
    segmentProtocol(playOrderActual[i][0], playOrderActual[i][1], currentGameTime, PRE_SEGMENT_DURATION, TOTAL_SEGMENT_DURATION);
    currentGameTime = millis();
  }

}


void segmentProtocol(int index1, int index2, unsigned long segmentStartTime, unsigned long preSegmentDuration, unsigned long totalSegmentDuration) {
  unsigned long segmentCurrentTime = segmentStartTime;
  unsigned long segmentPrepEndTime = segmentStartTime + preSegmentDuration;
  unsigned long segmentEndTime = segmentStartTime + totalSegmentDuration;
  BishiBashiPlayer *player1 = &playerRed;
  BishiBashiPlayer *player2 = &playerRed;

  //Assigning specfic player objects to a general object pointer
  if (playerRed.getIndex() == index1) {
    player1 = &playerRed;
  }
  else if (playerGreen.getIndex() == index1) {
    player1 = &playerGreen;
  }
  else if (playerBlue.getIndex() == index1) {
    player1 = &playerBlue;
  }
  else if (playerYellow.getIndex() == index1) {
    player1 = &playerYellow;
  }

  if (playerRed.getIndex() == index2) {
    player2 =  &playerRed;
  }
  else if (playerGreen.getIndex() == index2) {
    player2 = &playerGreen;
  }
  else if (playerBlue.getIndex() == index2) {
    player2 = &playerBlue;
  }
  else if (playerYellow.getIndex() == index2) {
    player2 = &playerYellow;
  }

  while (segmentPrepEndTime > segmentCurrentTime) {
#if DEBUG3
    Serial.println("waiting in preptime:");
    Serial.print("Player index: ");
    Serial.print((*player1).getIndex());

    switch ((*player1).getColour()) {
      case 0:
        Serial.print(" red");
        break;

      case 1:
        Serial.print(" green");
        break;

      case 2:
        Serial.print(" blue");
        break;

      case 3:
        Serial.print(" yellow");
        break;
    }
    Serial.println(" colour");
    Serial.println("VERSUS");
    Serial.print("Player index: ");
    Serial.print((*player2).getIndex());

    switch ((*player2).getColour()) {
      case 0:
        Serial.print(" red");
        break;

      case 1:
        Serial.print(" green");
        break;

      case 2:
        Serial.print(" blue");
        break;

      case 3:
        Serial.print(" yellow");
        break;
    }
    Serial.println(" colour");
    Serial.print("time left:");
    Serial.print(segmentPrepEndTime - segmentCurrentTime);
    Serial.println("...");
#endif

    //do something with neopixel
    segmentCurrentTime = millis();
  }
#if DEBUG2
  int i = 0;

#endif

  Serial.println("gameStart");
  while (segmentEndTime > segmentCurrentTime) {
    if ((*player1).playerIsPressed(LEFT)) {
      (*player1).addScore(1);
      (*player1).updateSpeedArray(segmentCurrentTime);
    }
    if ((*player1).playerIsPressed(RIGHT)) {
      (*player1).addScore(1);
      (*player1).updateSpeedArray(segmentCurrentTime);
    }
    if ((*player2).playerIsPressed(LEFT)) {
      (*player2).addScore(1);
      (*player2).updateSpeedArray(segmentCurrentTime);
    }

    if ((*player2).playerIsPressed(RIGHT)) {
      (*player2).addScore(1);
      (*player2).updateSpeedArray(segmentCurrentTime);
    }

    (*player1).updateSpeed(SPEED_SAMPLE_DURATION, segmentCurrentTime);
    (*player2).updateSpeed(SPEED_SAMPLE_DURATION, segmentCurrentTime);

    //    (*player1).addScore(LEFT);
    //    (*player1).addScore(RIGHT);
    //    (*player2).addScore(LEFT);
    //    (*player2).addScore(RIGHT);
    //    (*player1).getSpeed(200);
    //    (*player2).getSpeed(200 );
    segmentCurrentTime = millis();
#if DEBUG2
    i++;
    Serial.print("number of runs: ");
    Serial.println(i);
    Serial.print("THE CURRENT SEGMENT TIME IS:");
    Serial.println(segmentCurrentTime);

    Serial.print("THE TIME LEFT IN THIS SEGMENT IS: ");
    Serial.println(segmentEndTime - segmentCurrentTime);

    Serial.print("PLAYER ONES SCORE IS:");
    Serial.println((*player1).getScore());


    Serial.print("PLAYER TWO SCORE IS:");
    Serial.println((*player2).getScore());

#endif
#if DEBUG_SEGMENT_SPEED_PRINT
    Serial.print("player1 speed is");
    Serial.println((*player1).getSpeed());
    Serial.print("player2 speed is");
    Serial.println((*player2).getSpeed());

#endif

  }
#if DEBUG4
  Serial.println("END OF SEGMENT");
  Serial.print("PLAYER ONES SCORE IS:");
  Serial.println((*player1).getScore());


  Serial.print("PLAYER TWO SCORE IS:");
  Serial.println((*player2).getScore());
#endif

}


void RandomOrder(void) {

  int playerListSize = PLAYER_COUNT * REPEAT_PER_PLAYER;
  int playerList[playerListSize];
  int attempt, restartTrigger;

  do {                                                        // do-while loop 'A'
    restartTrigger = 0;                                       // set restartTrigger to FALSE to prevent infinite loop
    attempt = 0;                                              // set attempt to 0; scope: do-while loop 'A'

    /* creating playerList array */

    int playerIndex = 1;                                      // set the playerIndex to 1 i.e. start from the first player
    for (int i = 0; i < playerListSize; i++) {
      playerList[i] = playerIndex;
      if ((i + 1) % REPEAT_PER_PLAYER == 0) {
        playerIndex++;
      }                                                       // after REPEAT_PER_PLAYER copies, move on to the next playerIndex
    }

    /* creating randomized array */
    for (int i = 0; i < ROUND_COUNT_VAR; i++) {               // for loop 'B'
      int firstPlayerIndex, secondPlayerIndex;
      attempt = 0;                                            // set attempt to 0; scope: for loop 'B'

      do {
        firstPlayerIndex = rand() % playerListSize;
      } while (playerList[firstPlayerIndex] == 0);            // ensure that each element is only chosen once

      playOrderActual[i][0] = playerList[firstPlayerIndex];   // assign that random player to the first player
      playerList[firstPlayerIndex] = 0;                       // "cross out" the chosen element

      do {                                                    // do-while loop 'C'
        attempt++;                                            // taking note of the attempt to assign the second player
        do {
          secondPlayerIndex = rand() % playerListSize;
        } while (playerList[secondPlayerIndex] == 0);         // ensure that each element is only chosen once
        if (attempt > (playerListSize - 2 * i)) {
          break;                                              // break out of the do-while loop 'C' if all the options have been exhausted
        }
      } while (playerList[secondPlayerIndex] == playOrderActual[i][0]);         // ensure that the two players are different

      playOrderActual[i][1] = playerList[secondPlayerIndex];  // assign that random player to the second player
      playerList[secondPlayerIndex] = 0;                      // "cross out" the chosen element

      if (attempt > (playerListSize - 2 * i)) {
        restartTrigger = 1;                                   // set restartTrigger to TRUE
        break;                                                // break out of the for loop 'B' if all the options have been exhausted
      }
    }
  } while (restartTrigger == 1);                              // restart the entire process if the restartTrigger is set to TRUE

  for (int i = 0; i < ROUND_COUNT_VAR; i++) {
    if (playOrderActual[i][0] > playOrderActual[i][1]) {
      int temp = playOrderActual[i][0];
      playOrderActual[i][0] = playOrderActual[i][1];
      playOrderActual[i][1] = temp;
    }                                                         // for housekeeping purpose: keep the smaller index player on the left hand side of the array
  }
}

void updatePixels() {
//  pixelsCtrl.updatePixelsColors(currentTime);
//  pixels.show();
}
