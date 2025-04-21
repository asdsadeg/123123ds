#ifndef MINES_BOARD_H
#define MINES_BOARD_H

#include <stdbool.h>
#include <stdio.h>

#define MIN_ROW_COUNT 2
#define MIN_COLUMN_COUNT 2
#define MAX_ROW_COUNT 30
#define MAX_COLUMN_COUNT 30

typedef enum {
    CLOSED,
    OPEN,
    MARKED
} TileState;

typedef struct {
    bool is_mine;
    TileState tile_state;
    int value;
} Tile;

typedef struct {
    int row_count;
    int column_count;
    int mine_count;
    Tile *tiles[MAX_ROW_COUNT][MAX_COLUMN_COUNT];
} Board;

Board *create_board(int row_count, int column_count, int mine_count);
Board *create_our_board();
bool read_board_parameters(int* row_count, int* col_count, int* mine_count);
void destroy_board(Board *board);
bool is_game_solved(Board *board);
bool is_input_data_correct(Board *board, int input_row, int input_column);
void open_all_mines(Board *board);
void set_mines_randomly(Board *board);
void set_tile_values(Board *board);
bool is_mine_on(Board *board, int row, int column);
int count_neighbour_mines(Board *board, int row, int column);
void mark_all_mines(Board *board);

#endif //MINES_BOARD_H
