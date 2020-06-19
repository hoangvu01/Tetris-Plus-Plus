#include "display.h"

#include <curses.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIN_MIN_WIDTH 60
#define WIN_MIN_HEIGHT 40

void init_game_colour();
void printGrid(grid_t grid, WINDOW *game_window);
void printNext(state_t *cur, WINDOW *item_window);

int startScreen() {
  printf("\033[H\033[J");  // clears the terminal
  printf(
      " _____ _____ ___________ _____ _____ \n"
      "|_   _|  ___|_   _| ___ \\_   _/  ___|\n"
      "  | | | |__   | | | |_/ / | | \\ `--. \n"
      "  | | |  __|  | | |    /  | |  `--. \\\n"
      "  | | | |___  | | | |\\ \\ _| |_/\\__/ /\n"
      "  \\_/ \\____/  \\_/ \\_| \\_|\\___/\\____/ \n\n");
  printf("Press P to pause. \n");
  printf("Press Z / X to rotate. \n");
  printf("Press arrow keys to shift. \n\n");
  int levelNum;
  do {
    printf("Please select a level between 0 and 19: ");
    scanf("%d", &levelNum);
  } while (levelNum < 0 || levelNum > 19);

  char string[100];
  getchar();
  printf("Please enter a session token, leave blank to generate a new one: ");
  fgets(string, 100, stdin);
  string[strlen(string) - 1] = '\0';
  if (strlen(string) > 0) {
    srand(atoi(string));
  } else {
    int seed = time(0);
    srand(seed);
    printf("Your token is: %d \n", seed);
  }
  printf("Press enter to start game. \n");
  getchar();
  return levelNum;
}

WINDOW *init_display() {
  WINDOW *main_win = initscr();
  /* Set colour properties */
  if (!has_colors()) {
    fprintf(stderr, "You must enable colour terminal");
    endwin();
    exit(EXIT_FAILURE);
  }
  start_color();
  init_game_colour();
  wbkgd(stdscr, COLOR_BLACK);

  cbreak();
  noecho();
  curs_set(0);
  nodelay(main_win, true);
  keypad(stdscr, true);
  refresh();

  // TODO: write an offset as a function of window size
  int y_offset, x_offset, window_width;

  int maxy, maxx;
  getmaxyx(main_win, maxy, maxx);

  y_offset = 0 * maxy + 3;
  window_width = GWIDTH * 2 + 2;
  x_offset = maxx / 2 - window_width;

  WINDOW *game = subwin(main_win, GHEIGHT, window_width, y_offset, x_offset);
  touchwin(stdscr);
  wrefresh(game);
  return game;
}

void init_game_colour() {
  init_pair(0, COLOR_BLACK, COLOR_BLACK);
  init_pair(1, COLOR_CYAN, COLOR_CYAN);
  init_pair(2, COLOR_BLUE, COLOR_BLUE);
  init_pair(3, COLOR_GREEN, COLOR_GREEN);
  init_pair(4, COLOR_CYAN, COLOR_CYAN);
  init_pair(5, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(6, COLOR_MAGENTA, COLOR_MAGENTA);
  init_pair(7, COLOR_RED, COLOR_RED);
}

void printState(state_t *curr, WINDOW *game_window) {
  /* Calculate the position for scoreboard */
  int sub_maxy, sub_maxx, score_y_offset, score_x_offset;
  getmaxyx(game_window, sub_maxy, sub_maxx);
  getparyx(game_window, score_y_offset, score_x_offset);

  score_y_offset += sub_maxy / 4;
  score_x_offset += sub_maxx + 2;

  grid_t output = cloneGrid(curr->grid);

  for (int i = 0; i < 4; i++) {
    position_t cell;
    pplus(&cell, curr->pos, curr->block->spins[curr->rotation][i]);
    *(getSquare(output, cell)) =
        curr->block - curr->list + 1;  // set colour to block val
  }
  box(game_window, 0, 0);

  printGrid(output, game_window);
  mvprintw(getpary(game_window) + 1, score_x_offset, "High Score : %d\n",
           curr->highScore);
  mvprintw(score_y_offset++, score_x_offset, "Curr Score : %d\n",
           curr->level.score);
  mvprintw(score_y_offset++, score_x_offset, "Level : %d\n",
           curr->level.levelNum);
  mvprintw(score_y_offset++, score_x_offset, "Lines : %d\n", curr->totalLines);
  mvprintw(score_y_offset++, score_x_offset, "Next:");

  WINDOW *item_win = subwin(stdscr, 7, 14, ++score_y_offset, score_x_offset);
  box(item_win, 0, 0);
  touchwin(game_window);
  printNext(curr, item_win);

  wrefresh(item_win);
  refresh();
  freeGrid(output);
}

void printSquare(WINDOW *win, int colour_scheme) {
  for (int i = 0; i < 2; i++) {
    waddch(win, ' ' | COLOR_PAIR(colour_scheme));
  }
}

void printGrid(grid_t grid, WINDOW *w_game) {
  #ifdef PI_MODE
    update_led(grid);
  #endif

  for (int i = 2; i < GHEIGHT; i++) {
    wmove(w_game, i - 1, 1);
    for (int j = 0; j < GWIDTH; j++) {
      printSquare(w_game, grid[i][j]);
    }
  }
  touchwin(stdscr);
}

void printNext(state_t *curr, WINDOW *item_win) {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      bool isEmpty = true;
      wmove(item_win, i + 1, j * 2 + 1);
      for (int k = 0; k < 4; k++) {
        position_t cell = {2, 2};
        pplus(&cell, cell, curr->nextBlock->spins[0][k]);

        if (cell.x == j && cell.y == i) {
          isEmpty = false;
          printSquare(item_win, curr->nextBlock - curr->list + 1);
        }
      }
      if (isEmpty) wprintw(item_win, "  ");
    }
  }
}
