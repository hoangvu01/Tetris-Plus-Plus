#ifndef LEVELS_H
#define LEVELS_H

typedef struct 
{
	int levelNum;
	int score;
} level_t;

level_t initLevel(int level);

int framePerDrop(level_t level);

int get_level(level_t level);

int get_score(level_t level);

level_t update_level(level_t level, int totalLines, int linesCleared);

int calculate_score(int linesCleared, int levelNum);

#endif