#include "levels.h"
#include "testutils.h"
int main(){
	/* init level sets values correctly */
	level_t level = initLevel(8);
	testint(level.score, 0, "Init level sets values correctly");
	testint(level.levelNum, 8, "Init level sets values correctly");

	/* frames per drop is set Correctly */
	testint(framePerDrop(level), 8, "Frames per drop is set Correctly. ");
	level.levelNum = 12;
	testint(framePerDrop(level), 5, "Frames per drop is set Correctly. ");
	level.levelNum = 13;
	testint(framePerDrop(level), 4, "Frames per drop is set Correctly. ");
	level.levelNum = 15;
	testint(framePerDrop(level), 4, "Frames per drop is set Correctly. ");
	level.levelNum = 18;
	testint(framePerDrop(level), 3, "Frames per drop is set Correctly. ");

	/* update_level advances users correctly */
	level = update_level(level, 191, 4);
	testint(framePerDrop(level), 2, "update_level advances users correctly. ");
	testint(level.levelNum, 19, "update_level advances users correctly. ");
	testint(level.score, (19 + 1) * 1200, "update_level advances users correctly. ");


/* update_level stays if conditions are not met.  */
	level = update_level(level, 195, 0);
	testint(framePerDrop(level), 2, "update_level stays if conditions are not met.");
	testint(level.levelNum, 19, "update_level stays if conditions are not met.");
	testint(level.score, (19 + 1) * 1200, "update_level stays if conditions are not met.");


	return EXIT_SUCCESS;
}