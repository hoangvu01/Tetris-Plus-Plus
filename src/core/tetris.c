#include "tetris.h"
#include "state.h"


int main(int argc, char const *argv[]) {
  WINDOW *w = initscr();
  cbreak();
  nodelay(w, TRUE);
  noecho();

  state_t *curr = initState();
  bool hasMoving = false;

  timespec_t now, lastFrame;
  unsigned long frameNum = 0;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &lastFrame);

  while (1) {
    
    updateFrame(&now, &lastFrame, &frameNum);
    
    if (!hasMoving) {
      if (!spawnTetriminos(curr)) break;
      hasMoving = true;
    }
    printState(curr);
    detectInput(curr);

    if (frameNum % 28 == 0) hasMoving = dropPiece(curr);
  }

  freeState(curr);
  endwin();
  return EXIT_SUCCESS;
}

void updateFrame(timespec_t *now, timespec_t *lastFrame, unsigned long *frameNum) {
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, now);
  double delta = (now->tv_sec - lastFrame->tv_sec) * 1e6 +
                 (now->tv_nsec - lastFrame->tv_nsec) / 1e3;
  *lastFrame = *now;
  if (delta < 1e6 / FRAME_RATE) {
    usleep(1e6 / FRAME_RATE - delta);
  }
  *frameNum = *frameNum + 1;
}