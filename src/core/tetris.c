#include "display.h"
#include "game.h"

int main(int argc, char const *argv[]) {
  int levelNum = startScreen();
  
  WINDOW *game_win = init_display();

  state_t *curr = initState(levelNum);
  bool hasMoving = false;
  srand(time(0)); 
  timespec_t now, lastFrame;
  unsigned long frameNum = 0;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &lastFrame);

  while (1) {
    updateFrame(&now, &lastFrame, &frameNum);

    if (!hasMoving) {
      if (!spawnTetriminos(curr)) break;
      hasMoving = true;
    }
    printState(curr, game_win);
    detectInput(curr);

    if (frameNum % framePerDrop(curr->level) == 0) hasMoving = dropPiece(curr);
  }

  freeState(curr);
  endwin();

  if (curr->level.score > curr->highScore) writeHighScore(curr->level.score);
  printf("You scored %d points. \n", curr->level.score);
  printf("The high score is %d points. \n", readHighScore());

>>>>>>> dev
  return EXIT_SUCCESS;
}

