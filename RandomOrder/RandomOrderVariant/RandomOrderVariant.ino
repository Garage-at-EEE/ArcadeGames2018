/*
   Another version of random order
   Pro: can specify the number of times each player gets to play instead of repeating the entire set of matches
   Con: when the random function is not in our favour, the runtime is a lot longer than the original version
*/

#define PLAYER_COUNT        3
#define REPEAT_PER_PLAYER   50
#define ROUND_COUNT_VAR     (REPEAT_PER_PLAYER * PLAYER_COUNT / 2)    // just a round count, named differently from the previous version to prevent confusion

/* C and C++ cannot return an entire array so I have to resort to using a pointer instead */
int playOrderActual[ROUND_COUNT_VAR][2];

/*  to be placed in setup() */

/* define the function RandomOrder */

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

  for (int i = 0; i < ROUND_COUNT_VAR; i++){
    if (playOrderActual[i][0] > playOrderActual[i][1]){
      int temp = playOrderActual[i][0];
      playOrderActual[i][0] = playOrderActual[i][1];
      playOrderActual[i][1] = temp;
    }                                                         // for housekeeping purpose: keep the smaller index player on the left hand side of the array
  }
}
