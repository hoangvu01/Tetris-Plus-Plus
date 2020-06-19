#ifndef LEVELS_H
#define LEVELS_H

typedef struct {
  int levelNum;
  int score;
} level_t;

/*
 * Allocates a new level_t struct containing the level number as well as the
 * score.
 * @param: the initial level as specified by the user.
 */
level_t initLevel(int level);

/*
 * @returns: The number of frames to wait for before dropping a piece by 1 grid.
 * Determines the speed of the game.
 */
int framePerDrop(level_t level);

/*
 * Updates the information stored inside the level_t struct. Advances the user
 * to the next level if neccessary. Calls calculate_score automatically.
 * Determines the speed of the game.
 * @returns: the new level.
 */
level_t update_level(level_t level, int totalLines, int linesCleared);

/*
 * Calculates score gained using the number of lines cleared at once as well as
 * the current level number.
 * @returns: the score gained by clearing lines.
 */
int calculate_score(int linesCleared, int levelNum);

#endif
