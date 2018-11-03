#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>

#define PLAYER_COUNT  4
#define REPEAT_COUNT  1
#define ROUND_COUNT   (REPEAT_COUNT * PLAYER_COUNT * (PLAYER_COUNT-1) / 2)

/* C and C++ cannot return an entire array so I have to resort to using a pointer instead */
int playOrderActual[ROUND_COUNT][2];

/*  to be placed in setup() */

/* define the function RandomOrder */
void RandomOrder(void) {
  int playOrderOrdered[ROUND_COUNT][2];

  int rowAfterPlayeri = 0;

  for (int k = 1; k < PLAYER_COUNT; k++) {

        int j = 1;

    for (int i = rowAfterPlayeri; i < rowAfterPlayeri + (PLAYER_COUNT - k); i++) {
      /* start from the row after the row the previous player ended with */
      /* stop after (PLAYER_COUNT - k) rows i.e. P1 stops after 3 row, P2 stops after 2 rows, P3 stops after 1 row*/
      playOrderOrdered[i][0] = k;
      playOrderOrdered[i][1] = k + j;

              j++;
    }

    rowAfterPlayeri += PLAYER_COUNT - k;
  }

  int oneRoundLength = PLAYER_COUNT * (PLAYER_COUNT - 1) / 2;

  if (REPEAT_COUNT > 1){
    for (int n = 2; n <= REPEAT_COUNT; n++) {
        for (int i = 0; i < oneRoundLength; i++) {
            playOrderOrdered[i + (n-1) * oneRoundLength][0] = playOrderOrdered[i][0];
            playOrderOrdered[i + (n-1) * oneRoundLength][1] = playOrderOrdered[i][1];
        }
    }
  }

  for (int i = 0; i < ROUND_COUNT; i++){
    printf("Round %d:\t %d \t vs \t %d\n", (i+1), playOrderOrdered[i][0], playOrderOrdered[i][1]);
  }

  printf("RANDOMIZATION BEGINS \n");

for (int i = 0; i < ROUND_COUNT; i++){

        printf("i = %d \n", i);

        int randomIndex;

        do{
        randomIndex = (rand()%ROUND_COUNT);
        } while(playOrderOrdered[randomIndex][0] == 0);

        printf("randomIndex = %d \n", randomIndex);

        playOrderActual[i][0] = playOrderOrdered[randomIndex][0];
        playOrderActual[i][1] = playOrderOrdered[randomIndex][1];

        printf("playOrderActual[i][0] = %d \n", playOrderActual[i][0]);
        printf("playOrderActual[i][1] = %d \n", playOrderActual[i][1]);

        playOrderOrdered[randomIndex][0] = 0;
  }

  for (int i = 0; i < ROUND_COUNT; i++){
    printf("Round %d:\t %d \t vs \t %d\n", (i+1), playOrderActual[i][0], playOrderActual[i][1]);
  }

}

int main(void){
    RandomOrder();
}
