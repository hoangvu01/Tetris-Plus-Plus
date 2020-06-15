#include <time.h>

#include "display.h"
#include "game.h"

int main(int argc, char const *argv[]) {
  int levelNum = startScreen();
  srand(time(0));

  startGame(levelNum);
  return EXIT_SUCCESS;
}

