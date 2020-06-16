#include <time.h>
#include "play.h"
#include "display.h"
#include "state.h"
#include "levels.h"

#define FRAME_RATE 60

typedef struct timespec timespec_t;

void startGame(int levelNum);
void updateFrame(timespec_t *now, timespec_t *lastFrame, unsigned long *frameNum);
/* This parameter has been trained over the following config:
 * max_piece = 500, iterations = 5
 */
static param_state_t param = {0.553276, 0.271804, 0.753433, 0.228793, 191};
static param_state_t *param_p = &param;

int main(void) {
  int levelNum = startScreen();
  srand(time(0));
  startGame(levelNum);
  return EXIT_SUCCESS;
}

void startGame(int levelNum) {
  WINDOW *game_win = init_display();
  state_t *curr = initState(levelNum);
  bool hasMoving = false;

  timespec_t now, lastFrame;
  unsigned long frameNum = 0;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &lastFrame);

  while (1) {
    updateFrame(&now, &lastFrame, &frameNum);

    if (!hasMoving) {
      if (!spawnTetriminos(curr)) break;
      block_t best_block;
      immutable_best_move(curr, param_p, &best_block, 0);
      set_state_by_block(curr, &best_block);
      curr->pos.y = 2;
      hasMoving = true;
    }
    printState(curr, game_win);

    if (frameNum % framePerDrop(curr->level) == 0) hasMoving = dropPiece(curr);
  }

  freeState(curr);
  endwin();

  if (curr->level.score > curr->highScore) writeHighScore(curr->level.score);
  printf("You scored %d points. \n", curr->level.score);
  printf("The high score is %d points. \n", readHighScore());
}

void updateFrame(timespec_t *now, timespec_t *lastFrame,
                 unsigned long *frameNum) {
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, now);
  double delta = (now->tv_sec - lastFrame->tv_sec) * 1e6 +
                 (now->tv_nsec - lastFrame->tv_nsec) / 1e3;
  *lastFrame = *now;
  if (delta < 1e6 / FRAME_RATE) {
    usleep(1e6 / FRAME_RATE - delta);
  }
  *frameNum = *frameNum + 1;
}
