#include <ncurses.h>
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>

#include "display.h"

#define WIN_MIN_WIDTH 60
#define WIN_MIN_HEIGHT 40

void print_grid(grid_t, WINDOW *);
void print_next(state_t *);

WINDOW *init_display() {
  WINDOW *main = initscr();
  cbreak();
  noecho();
  nodelay(main, TRUE);
  box(main, 0, 0);
  curs_set(0);
  refresh();

  // TODO: write an offset as a function of window size
  int y_offset = 3, x_offset = 3;
  WINDOW *game = subwin(main, GHEIGHT, GWIDTH * 2 + 2, y_offset, x_offset);
  box(game, '*', '*');
  touchwin(stdscr);
  wrefresh(game);
  return game;
}


void print_state(state_t *curr, WINDOW *game_window) {
  grid_t output = cloneGrid(curr->grid);

  for (int i = 0; i < 4; i++) {
    position_t cell;
    pplus(&cell, curr->pos, curr->block->spins[curr->rotation][i]);
    *(getSquare(output, cell)) =
        curr->block - curr->list + 1;  // set colour to block val
  }
  print_grid(output, game_window);
  mvprintw(0, 35, "Level : %-02d\n", curr->level.levelNum);
  mvprintw(1, 35, "Score : %-10d\n", curr->level.score);
  mvprintw(2, 35, "Lines : %-10d\n", curr->totalLines);
  print_next(curr);

  refresh();
  freeGrid(output);
}

void print_grid(grid_t grid, WINDOW *w_game) {
  for (int i = 2; i < GHEIGHT; i++) {
    wmove(w_game, i - 1, 1);
    for (int j = 0; j < GWIDTH; j++) {
      if (grid[i][j] == 0)
        wprintw(w_game, "  ");
      else
        wprintw(w_game, "%d ", grid[i][j]);
    }
  }
  touchwin(stdscr);
}

void print_next(state_t *curr) {
  move(3, 35);
  printw("next:\n");
  for (int i = 0; i < 5; i++) {
    mvprintw(i + 4, 41, "|");
    for (int j = 0; j < 5; j++) {
      bool isEmpty = true;
      for (int k = 0; k < 4; k++) {
        position_t cell = {2, 2};
        pplus(&cell, cell, curr->nextBlock->spins[0][k]);

        if (cell.x == j && cell.y == i) {
          isEmpty = false;
          printw("%d ", curr->nextBlock - curr->list);
        }
      }
      if (isEmpty) printw("  ");
    }
    printw("|\n");
  }
}

