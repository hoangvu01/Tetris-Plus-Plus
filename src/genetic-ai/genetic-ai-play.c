#include <time.h>
#include "genetic-ai-play.h"
#include "display.h"
#include "state.h"
#include "levels.h"

#define FRAME_RATE 60

typedef struct timespec timespec_t;

void startGameWithAI(int levelNum, param_state_t *param_p, bool is_conservative);
void updateFrame(timespec_t *now, timespec_t *lastFrame, unsigned long *frameNum);
/* This parameter has been trained over the following config:
 * max_piece = 500, iterations = 5
 */
/* the conservative */
static param_state_t param_conservative = {0.553276, 0.271804, 0.753433, 0.228793, 191};
/* the risky */
static param_state_t param_risky = {0.684983, 0.029138, 0.585340, 0.432812, 531};
//static param_state_t param_risky = {0.663952, 0.048066, 0.679833, 0.307709, 0.381876, 541};
static param_state_t *param_conservative_p = &param_conservative;
static param_state_t *param_risky_p = &param_risky;

int main(int argc, char **argv) {
  int levelNum = startScreen();
  if (strcmp(argv[1], "conservative") == 0) {
    startGameWithAI(levelNum, param_conservative_p, true);
  } else if (strcmp(argv[1], "risky") == 0) {
    startGameWithAI(levelNum, param_risky_p, false);
  }

  return EXIT_SUCCESS;
}

void startGameWithAI(int levelNum, param_state_t *param_p, bool is_conservative) {
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
      immutable_best_move(curr, param_p, &best_block, 0, is_conservative);
      set_state_by_block(curr, &best_block);
      curr->pos.y = 2;
      hasMoving = true;
    }
    printState(curr, game_win);

    if (frameNum % framePerDrop(curr->level) == 0) hasMoving = dropPiece(curr);
  }

  freeState(curr);
  endwin();
  free(game_win);

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
