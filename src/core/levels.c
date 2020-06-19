#include "levels.h"

level_t initLevel(int levelNum) {
  level_t l;
  l.score = 0;
  l.levelNum = levelNum;
	return l;
}

int framePerDrop(level_t l) {
  if (l.levelNum < 9) return 48 - 5 * l.levelNum;
  if (l.levelNum == 9) return 6;
  if (l.levelNum >= 10 && l.levelNum <= 12) return 5;
  if (l.levelNum >= 13 && l.levelNum <= 15) return 4;
  if (l.levelNum >= 16 && l.levelNum <= 18) return 3;
  if (l.levelNum >= 19 && l.levelNum <= 28) return 2;
  return 1;
}

level_t update_level(level_t level, int totalLines, int linesCleared) {
	level_t newL = level;
	if (totalLines >= 10 * (level.levelNum + 1)){
		newL.levelNum++;
	}
	newL.score +=calculate_score(linesCleared, newL.levelNum);
	return newL;
}

int calculate_score(int linesCleared, int levelNum) {
  int scoreTable[5] = {0, 40, 100, 300, 1200};
  return scoreTable[linesCleared] * (levelNum + 1);
}
