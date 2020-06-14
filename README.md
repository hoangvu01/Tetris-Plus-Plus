# Tetris Plus Plus

An extension on the classic tetris game designed to help you get through quarentine, with features designed specifically to to help you cope with quarentine life. 

**Tetris Sport**: 
Need exercise but can't go outside? No problem. Tetris Sport is a way  to exercise indoors in a fun way. We used a Raspberry Pi and some accelerometres to allow you to control a Tetris game by moving around. 


**Tetris Buddy**: 
Games are more fun when there is someone to compete with, but in the quarentine environment it is difficult to have friends over. We used machine learning to create someone to play Tetris with. See if you can beat him at his own game. 

### Layout
Members should commit to the tetris-plus-plus repository directly, the master branch of said repository will be synced with the main repo. 

    /tetris-plus-plus
    |
    |____/bin        (the final executable file)
    |
    |
    |____/src        (every source file, .c and .h)
    |    |
    |    |__/core    (Basic implementation of the game. )
    |
    |
    |____/obj        (where the generated .o files will be)
    |
    |____/lib        (any library dependences)
    |
    |____Makefile    (make instructions file)
    |
    |____README      (general readme of the project)
