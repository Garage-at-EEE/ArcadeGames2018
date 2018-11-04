#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>

#define PLAYER_COUNT        4
#define REPEAT_PER_PLAYER   3
#define ROUND_COUNT_VAR     (REPEAT_PER_PLAYER * PLAYER_COUNT / 2)

/* C and C++ cannot return an entire array so I have to resort to using a pointer instead */
int playOrderActual[ROUND_COUNT_VAR][2];

/*  to be placed in setup() */

/* define the function RandomOrder */

void RandomOrder(void) {

  int playerListSize = PLAYER_COUNT * REPEAT_PER_PLAYER;
  int playerList[playerListSize];
  int attempt, restartTrigger;

  do {
    restartTrigger = 0;
    attempt = 0;
    int playerIndex = 1;
    /* creating playerList array */
    for (int i = 0; i < playerListSize; i++) {
      // printf("i = %d \n", i);

      playerList[i] = playerIndex;
      // printf("playerList[i] = %d \n", playerList[i]);
      if ((i + 1) % REPEAT_PER_PLAYER == 0) {
        playerIndex++;
      }
      /* each player has REPEAT_PER_PLAYER duplicates */
    }

    /* creating randomized array */
    int firstPlayer, secondPlayer;


    for (int i = 0; i < ROUND_COUNT_VAR; i++) {
      printf("i = %d \t", i);

      int firstPlayerIndex, secondPlayerIndex;
      attempt = 0;

      do {
        firstPlayerIndex = rand() % playerListSize;
      } while (playerList[firstPlayerIndex] == 0);

      playOrderActual[i][0] = playerList[firstPlayerIndex];
      playerList[firstPlayerIndex] = 0;
      printf("playOrderActual[i][0] = %d \t", playOrderActual[i][0]);

      do {
        attempt++;
        printf("\nattempt = %d ...\t", attempt);
        do {
          secondPlayerIndex = rand() % playerListSize;
        } while (playerList[secondPlayerIndex] == 0);
        if (attempt > (playerListSize - 2 * i)) {
          break;
        }
      } while (playerList[secondPlayerIndex] == playOrderActual[i][0]);

      playOrderActual[i][1] = playerList[secondPlayerIndex];
      playerList[secondPlayerIndex] = 0;
      printf("playOrderActual[i][1] = %d \n", playOrderActual[i][1]);

      if (attempt > (playerListSize - 2 * i)) {
        printf("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        printf("unable to assign\n");
        printf("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        restartTrigger = 1;
        break;
      }
    }
  } while (restartTrigger == 1);

  for (int i = 0; i < ROUND_COUNT_VAR; i++){
    if (playOrderActual[i][0] > playOrderActual[i][1]){
      int temp = playOrderActual[i][0];
      playOrderActual[i][0] = playOrderActual[i][1];
      playOrderActual[i][1] = temp;
    }                                                         // for housekeeping purpose: keep the smaller index player on the left hand side of the array
  }

}



int main(void){
    srand((unsigned)time(NULL));
    RandomOrder();

    for (int i = 0; i < ROUND_COUNT_VAR; i++){
        printf("Round %d:\t", i+1);
        printf("Player %d \t vs \t Player %d \n", playOrderActual[i][0], playOrderActual[i][1]);
    }
}
