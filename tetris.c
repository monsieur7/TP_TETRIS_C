#ifndef TETRIS_C
#define TETRIS_C

// Path: tetris.c
#include "tetris.h"
#include <stdio.h>
#include <string.h> // memset
#include <stdlib.h> // malloc
// CONSTATNTS :
//  PIECES CONSTANTS (4*4 arrays)

const char SQUARE[4][4] = {
    {0, 0, 0, 0},
    {0, 1, 1, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0}};
const char BAR[4][4] = {
    {0, 0, 0, 0},
    {1, 1, 1, 1},
    {0, 0, 0, 0},
    {0, 0, 0, 0}};
const char TEE[4][4] = {
    {0, 0, 0, 0},
    {0, 1, 0, 0},
    {1, 1, 1, 0},
    {0, 0, 0, 0}};
const char ZEE[4][4] = {
    {0, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0}};
const char ELLE[4][4] = {
    {0, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 0}};
const char JAY[4][4] = {
    {0, 0, 0, 0},
    {0, 0, 1, 0},
    {0, 1, 1, 0},
    {0, 1, 0, 0}};
const char ESS[4][4] = {
    {0, 0, 0, 0},
    {0, 0, 1, 1},
    {0, 1, 1, 0},
    {0, 0, 0, 0}};

// UTILS :
void copy_block(char dest[4][4], const char src[4][4])
{
    memcpy(dest, src, sizeof(char) * 4 * 4);
}
void init_board(TETRIS *t)
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            t->board[i][j] = 0;
        }
    }
    t->current_block = (BLOCK_INFO **)(malloc(sizeof(BLOCK_INFO *) * 50)); // allocate memory for 10 blocks

    if (t->current_block == NULL)
    {
        // Handle allocation failure
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 50; i++)
    {
        t->current_block[i] = malloc(sizeof(BLOCK_INFO));
        if (t->current_block[i] == NULL)
        {
            // Handle allocation failure
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }
        memset(t->current_block[i], 0, sizeof(BLOCK_INFO));
    }
    t->current_pt = 0;
}

void print_board(TETRIS *t)
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            printf("%d ", t->board[i][j]);
        }
        printf("\n");
    }
    printf("\r\n");
}

void print_block(char block[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%d ", block[i][j]);
        }
        printf("\n");
    }
}

void rotate_block(TETRIS *t, int x, int y)
{
    char *temp = malloc(sizeof(char) * 4 * 4);
    if (temp == NULL)
    {
        // Handle allocation failure
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    memset(temp, 0, sizeof(char) * 4 * 4);

    // copy block to temp
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            temp[i * 4 + j] = t->board[i + y][j + x];
        }
    }

    // clear old block
    remove_block(t, temp, x, y);

    // rotate temp
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            t->board[i + y][j + x] = temp[(3 - j) * 4 + i];
        }
    }
    t->current_block[t->current_pt]->x = x;
    t->current_block[t->current_pt]->y = y;
    copy_block(t->current_block[t->current_pt]->block, temp);
    t->current_pt++; // add block to current_block

    free(temp); // Don't forget to free the allocated memory
}
void place_block(TETRIS *t, const char block[4][4], int x, int y)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            t->board[i + y][j + x] = block[i][j];
        }
    }
    t->current_block[t->current_pt]->x = x;
    t->current_block[t->current_pt]->y = y;
    copy_block(t->current_block[t->current_pt]->block, block);
    t->current_pt++; // add block to current_block
}
/*
RETURN 0 if block cannot move
*/
int check_to_move(TETRIS *t, const char block[4][4], int x, int y)
{
    int end_of_line = 0;
    for (int i = 3; i >= 0; i--)
    {
        if (end_of_line == 1)
        {
            break;
        }
        for (int j = 0; j < 4; j++)
        {
            // //printf("block[%d][%d] = %d, (i + y + 1) = %d\n", i, j, block[i][j], (i + y + 1));

            // check if block is at the bottom of the board
            if (block[i][j] == 1 && ((i + y + 1) >= HEIGHT))
            {
                // printf("block is at the bottom of the board\n");
                return 0;
            }

            // if there is a 1 in the block, this y line is the last line of the block to check
            if (block[i][j] == 1)
            {
                end_of_line = 1; // break out of the loop by setting i to -1
            }
            // printf("i = %d, j = %d, block[%d][%d] = %d, board[%d][%d] = %d\n",
            //      i, j, i, j, block[i][j], i + y + 1, j + x, t->board[i + y + 1][j + x]);
            if (block[i][j] == 1 && t->board[i + y + 1][j + x] == 1)
            { // check if block is on top of another block (+1 )
                return 0;
            }
        }
    }
    return 1;
}
void remove_block(TETRIS *t, const char block[4][4], int x, int y)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            t->board[i + y][j + x] = 0;
        }
    }
    t->current_pt--; // remove block from current_block
}
char *get_block(TETRIS *t, int x, int y)
{
    char *block = malloc(sizeof(char) * 4 * 4);
    // copy block to temp
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            block[i * 4 + j] = t->board[i + y][j + x];
        }
    }
    return block;
}

void move_block(TETRIS *t, const char block[4][4], int x, int y)
{
    if (check_to_move(t, block, x, y) == 1)
    {
        // only move if block can move and only move != 0 items
        for (int i = 3; i >= 0; i--)
        {
            for (int j = 0; j < 4; j++)
            {
                if (block[i][j] != 0)
                {
                    t->board[i + y + 1][j + x] = block[i][j];
                    t->board[i + y][j + x] = 0;
                }
            }
        }
        t->current_block[t->current_pt - 1]->y += 1; // move block down by 1
        // printf("move_block: %d\n", t->current_block[t->current_pt - 1]->y);
    }
}
void drop(TETRIS *t)
{
    int x = t->current_pt - 1; // Start from the last block
    int res = 1;
    while (x >= 0)
    {

        while (res == 1)
        {
            move_block(t, t->current_block[x]->block, t->current_block[x]->x, t->current_block[x]->y);
            //  print_board(t);
            // printf("x = %d, y = %d\n", t->current_block[x]->x, t->current_block[x]->y);
            res = check_to_move(t, t->current_block[x]->block, t->current_block[x]->x, t->current_block[x]->y);
            // printf("res = %d\n", res);
        }
        x--;
    }
}

void check_line(TETRIS *t)
{

    int line = 0;
    for (int i = 0; i < HEIGHT; i++) // check for each line
    {
        int count = 0;
        for (int j = 0; j < WIDTH; j++)
        {
            if (t->board[i][j] == 1)
            {
                count++;
            }
        }
        if (count == WIDTH) // if a line is full
        {
            line++;            // increase line count
            delete_line(t, i); // delete the line
        }
    }
    // printf("line: %d\n", line);
}
void delete_line(TETRIS *t, int y)
{
    for (int i = y; i > 0; i--) // move all lines above the deleted line down by 1
    {
        for (int j = 0; j < WIDTH; j++)
        {
            t->board[i][j] = t->board[i - 1][j];
        }
    }
    for (int j = 0; j < WIDTH; j++) // clear the line to delete
    {
        t->board[y][j] = 0;
    }
}
void check_game_over(TETRIS *t)
{
    for (int i = 0; i < WIDTH; i++)
    {
        if (t->board[0][i] == 1)
        {
            // printf("GAME OVER\n");
            exit(0);
        }
    }
}

void new_block(TETRIS *t)
{
    // choose a random block
    int r = rand() % 7;
    switch (r)
    {
    case 0:
        place_block(t, SQUARE, WIDTH / 2 - 2, 0);
        copy_block(t->current_block[t->current_pt]->block, SQUARE);
        break;
    case 1:
        place_block(t, BAR, WIDTH / 2 - 2, 0);
        copy_block(t->current_block[t->current_pt]->block, BAR);
        break;
    case 2: // TEE
        place_block(t, TEE, WIDTH / 2 - 2, 0);
        copy_block(t->current_block[t->current_pt]->block, TEE);
        break;
    case 3: // ZEE
        place_block(t, ZEE, WIDTH / 2 - 2, 0);
        copy_block(t->current_block[t->current_pt]->block, ZEE);
        break;
    case 4: // ELLE
        place_block(t, ELLE, WIDTH / 2 - 2, 0);
        copy_block(t->current_block[t->current_pt]->block, ELLE);
        break;
    case 5: // JAY
        place_block(t, JAY, WIDTH / 2 - 2, 0);
        copy_block(t->current_block[t->current_pt]->block, JAY);
        break;
    case 6: // ESS
        place_block(t, ESS, WIDTH / 2 - 2, 0);
        copy_block(t->current_block[t->current_pt]->block, ESS);
        break;
    default:
        break;
    }
    // increment current_pt and block list
    t->current_block[t->current_pt]->x = WIDTH / 2 - 2;
    t->current_block[t->current_pt]->y = 0;
    t->current_pt++;
}
#endif // TETRIS_C
