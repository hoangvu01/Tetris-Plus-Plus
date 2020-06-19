# Tetris Plus Plus

An extension on the classic tetris game. 

**Tetris Sports:**
Need exercise but can't go outside? No problem. Tetris Sport is a way  to exercise indoors in a fun way. We used a Raspberry Pi and some accelerometres to allow you to control a Tetris game by moving around.

**Tetris Rhythms:**
Want another way to play the game? Tetris rhythms allow you to play tetris following the beats of your favourite music!

**Tetris Buddy:**
Games are more fun when there is someone to compete with, but in the quarentine environment it is difficult to have friends over. We used machine learning to create someone to play Tetris with. See if you can beat him at his own game.



# Make and Run

### Terminal
To play it on terminal, `make core` in the root directory. Run `bin/tetris`.

The player has to pick a level to start with, then chooses to put in a randomnumber as a seed. The higher the level, the faster the game. 

The control keys are shown on the welcome screen:  press *P* to pause, press *Z / X* to rotate the block, press arrow keys to move left or right.

### AI Training
To train the AI implemented by genetic algorithm, `make genetic-train` in the root directory. Run `bin/train`.

The training progress will be displayed in the command line. After the training of each generation of parameters, the result willbe written to a text file named `trainingprogress.txt` in `/bin`.  You can continue training from the text file by running `bin/train trainingprogress.txt`.

To watch the genetic AI play, `make genetic` in the root directory. If you want to see the conservative AI plays, run `bin/genetic-ai-play conservative`. If you want to see the risky AI plays, run `bin/genetic-ai-play risky`. To train the AI implemented by reinforcement learning, `make rl-trainin` the root directory to generate the executable and run `bin/rltrain` to watch as it trains.

### PI
The raspberry PI module only compiles on a raspian OS since it requires `wiringPi` library.

To play Tetris on PI, go to `src/core/include/state.h` and uncomment `#define PI_MODE`. Choose a mode and change the line below into `#define GAME_MODE <game mode you chose>`. Then `make pi` in the root directory and run `bin/tetrispi`. The wiring specifications are provided in the source code files in `src/pi` directory.

# Layout
    /tetris-plus-plus
    |
    |____/bin        (the final executable file)
    |
    |
    |____/src        (every source file, .c and .h)
    |    |
    |    |__/core        (Basic implementation of the game. )
    |    |__/pi          (Raspberry Pi implementation of the game. )
    |    |__/genetic_ai  (Implementation for AI using genetic algorithm. )
    |    |__/rl_al       (Implementation for AI using reinforcement learning. )
    |
    |____/obj        (where the generated .o files will be)
    |
    |____/lib        (any library dependences)
    |
    |____Makefile    (make instructions file)
    |
    |____README      (general readme of the project)
