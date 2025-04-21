#ifndef MINES_BOARD_H
#define MINES_BOARD_H
#include <stdbool.h>
#define MAX_ROW_COUNT 30
#define MAX_COLUMN_COUNT 30

typedef enum {
    CLOSED,
    OPEN,
    MARKED
} TileState;

typedef struct {
    bool is_mine;                /* Records if mine is on the Tile */
    TileState tile_state;        /* Enum for status of the Tile state */
    int value;                   /* Number of neighbour tiles with mines,
                                    or -1 if the tile contains a mine */
} Tile;

typedef struct {
    int row_count;                                  /* Number of rows in the Board */
    int column_count;                               /* Number of columns in the Board */
    int mine_count;                                 /* Number of mines in the Board */
    Tile *tiles[MAX_ROW_COUNT][MAX_COLUMN_COUNT];   /* 2-dimensional struct array of the tiles */
} Board;

Board *create_board(int row_count, int column_count, int mine_count);
Board *create_interactive_board();
bool input_board_parameters(int* row_count, int* col_count, int* mine_count);
void destroy_board(Board *board);
bool is_game_solved(Board *board);
bool is_input_data_correct(Board *board, int input_row, int input_column);
void open_all_mines(Board *board);
void set_mines_randomly(Board *board, int input_row, int input_column);
//DECLARATION FOR AVOIDING WARNINGS//
void set_tile_values(Board *board);
bool is_mine_on(Board *board, int row, int column);
int count_neighbour_mines(Board *board, int row, int column);
void mark_all_mines(Board *board);
int generate_random_coordinates(int upper_range);

#endif //MINES_BOARD_H
