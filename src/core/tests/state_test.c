#include "state.h"
#include "testutils.h"

int main(){
	/* initState sets levels and totalLines correctly */
	state_t *state = initState(12);
	testint(state->totalLines, 0, "initState sets totalLines correctly. ");
	testint(state->level.levelNum, 12, "initState sets levels correctly. ");

	/* spawnTetriminos spawns in the right place */
	testbool(spawnTetriminos(state), "Can spawn in empty field");
	testint(state->pos.x, 5, "spawnTetriminos spawns in the right place. ");
	testint(state->pos.y, 2, "spawnTetriminos spawns in the right place. ");

	/* Can drop piece in empty field */
	testbool(dropPiece(state), "Can drop piece in empty field");

}