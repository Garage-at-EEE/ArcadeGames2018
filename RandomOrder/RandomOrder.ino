#define PLAYER_COUNT  4
#define REPEAT_COUNT  1
#define ROUND_COUNT   (REPEAT_COUNT * PLAYER_COUNT * (PLAYER_COUNT-1) / 2)

/* C and C++ cannot return an entire array so I have to resort to defining this as a global instead */
int playOrderActual[ROUND_COUNT][2];

/*  to be placed in void setup(){...} */

/* define the function RandomOrder */
void RandomOrder(void) {

  /* creating template array */

  int playOrderOrdered[ROUND_COUNT][2]; /* define a template array */

  int rowAfterPlayeri = 0; /* counting the row of the array the next player will start from */

  for (int k = 1; k < PLAYER_COUNT; k++) { /* first player index = k */

    int j = 1; /* second player's index relative to the first player */

    for (int i = rowAfterPlayeri; i < rowAfterPlayeri + (PLAYER_COUNT - k); i++) {
      /* start from the row after the row the previous player ended with */
      /* stop after (PLAYER_COUNT - k) rows i.e. P1 stops after 3 row, P2 stops after 2 rows, P3 stops after 1 row*/

      playOrderOrdered[i][0] = k; /* assign k to first player's index in the template array */
      playOrderOrdered[i][1] = k + j; /*assign k + j to second player's index in the template array */

      j++; /* increment j */
    }

    rowAfterPlayeri += PLAYER_COUNT - k; /* assigning the row of the array the next player will start from */
  }

  int oneRoundLength = PLAYER_COUNT * (PLAYER_COUNT - 1) / 2; /* = binom(PLAYER_COUNT,2) */

  /* accounting for games with repeat matches i.e. the same pair meets more than once */
  if (REPEAT_COUNT > 1) {
    for (int n = 2; n <= REPEAT_COUNT; n++) { /* on the nth repeat */
      for (int i = 0; i < oneRoundLength; i++) { /* on each row, i */
        playOrderOrdered[i + (n - 1) * oneRoundLength][0] = playOrderOrdered[i][0];
        playOrderOrdered[i + (n - 1) * oneRoundLength][1] = playOrderOrdered[i][1];
        /* copy the ith row to the bottom */
      }
    }
  }

  /* creating randomized play order */
  for (int i = 0; i < ROUND_COUNT; i++) { /* on the ith row of the randomized array */
    int randomIndex;

    do {
      randomIndex = (rand() % ROUND_COUNT);
    } while (playOrderOrdered[randomIndex][0] == 0); /* ensuring that each match from a template is chosen only once */

    playOrderActual[i][0] = playOrderOrdered[randomIndex][0];
    playOrderActual[i][1] = playOrderOrdered[randomIndex][1];
    /* retrieving the row from the template array to the randomized array */

    playOrderOrdered[randomIndex][0] = 0; /* "crossing out" the chosen match */
  }
}
