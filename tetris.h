#ifndef TETRIS_H
#define TETRIS_H

#define WIDTH 12
#define HEIGHT 12
typedef struct BLOCK_INFO
{
    char block[4][4];
    int x;
    int y;
} BLOCK_INFO;

typedef struct TETRIS
{
    char board[HEIGHT][WIDTH];
    BLOCK_INFO **current_block;
    int current_pt;
} TETRIS;

void init_board(TETRIS *t);
void print_board(TETRIS *t);
void print_block(char block[4][4]);
void rotate_block(TETRIS *t, int x, int y);

// x,y is the position of the upper left corner of the block
void place_block(TETRIS *t, const char block[4][4], int x, int y);

int check_to_move(TETRIS *t, const char block[4][4], int x, int y);
void move_block(TETRIS *t, const char block[4][4], int x, int y);
void remove_block(TETRIS *t, const char block[4][4], int x, int y);
char *get_block(TETRIS *t, int x, int y);

void drop(TETRIS *t);

void block_down(TETRIS *t);         // USELESS
void block_down_all(TETRIS *t);     // USELESS
void new_block(TETRIS *t);          // AT RANDOM
void check_game_over(TETRIS *t);    // TODO
void check_line(TETRIS *t);         // TODO
void delete_line(TETRIS *t, int y); // TODO

void copy_block(char dest[4][4], const char src[4][4]);
extern const char SQUARE[4][4];
extern const char BAR[4][4];
extern const char TEE[4][4];
extern const char ZEE[4][4];
extern const char ELLE[4][4];
extern const char JAY[4][4];
extern const char ESS[4][4];
#endif // TETRIS_H
