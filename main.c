#include <stdio.h>
#include "tetris.h"

int main(void)
{
    TETRIS t;
    init_board(&t);
    print_board(&t);

    // MAIN GAME LOOP
    while (1)
    {
        // generate a new block
        new_block(&t);
        // print_board(&t);
        //  move the block down
        drop(&t);
        print_board(&t);

        // check if the game is over
        check_game_over(&t);
        // wait for user input
        char input;
        scanf(" %c", &input);
    }
    return 0;
}