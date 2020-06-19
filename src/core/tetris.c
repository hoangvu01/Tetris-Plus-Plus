#include <time.h>
#include "display.h"
#include "game.h"

int main(int argc, char const *argv[]) {
  int levelNum = startScreen();

  startGame(levelNum);
  return EXIT_SUCCESS;
}

