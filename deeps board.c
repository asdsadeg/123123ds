#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "board.h"

bool is_mine_on(Board *board, int row, int column) {
    assert(board != NULL);
    return row >= 0 && row < board->row_count && column >= 0
           && column < board->column_count
           && board->tiles[row][column]->is_mine;
}

int count_neighbour_mines(Board *board, int row, int column) {
    assert(board != NULL);
    int count = 0;
    for (int drow = -1; drow <= 1; drow++) {
        for (int dcolumn = -1; dcolumn <= 1; dcolumn++) {
            if (is_mine_on(board, row + drow, column + dcolumn)) {
                count++;
            }
        }
    }
    return count;
}

void set_tile_values(Board *board) {
    assert(board != NULL);
    for (int row = 0; row < board->row_count; row++) {
        for (int column = 0; column < board->column_count; column++) {
            if (board->tiles[row][column]->is_mine) {
                board->tiles[row][column]->value = -1;
            } else {
                board->tiles[row][column]->value =
                        count_neighbour_mines(board, row, column);
            }
        }
    }
}

void mark_all_mines(Board *board) {
    assert(board != NULL);
    for (int row = 0; row < board->row_count; row++) {
        for (int column = 0; column < board->column_count; column++) {
            if (board->tiles[row][column]->tile_state == CLOSED) {
                board->tiles[row][column]->tile_state = MARKED;
            }
        }
    }
}

int generate_random_coordinates(int upper_range) {
    return rand() % upper_range;
}

void set_mines_randomly(Board *board, int first_click_row, int first_click_column) {
    assert(board != NULL);
    int board_mine_count = 0;
    srand(time(NULL));
    while (board_mine_count != board->mine_count) {
        int random_row = generate_random_coordinates(board->row_count);
        int random_column = generate_random_coordinates(board->column_count);

        if (board->tiles[random_row][random_column]->is_mine == false) {
            board->tiles[random_row][random_column]->is_mine = true;
            board_mine_count++;
        }
    }
}

// board.c
bool read_board_parameters(int* row_count, int* col_count, int* mine_count) {
    printf("Enter number of rows (minimum %d): ", MIN_ROW_COUNT);
    if (scanf("%d", row_count) != 1 || *row_count < MIN_ROW_COUNT) {
        fprintf(stderr, "Invalid row count! Must be at least %d\n", MIN_ROW_COUNT);
        return false;
    }

    printf("Enter number of columns (minimum %d): ", MIN_COLUMN_COUNT);
    if (scanf("%d", col_count) != 1 || *col_count < MIN_COLUMN_COUNT) {
        fprintf(stderr, "Invalid column count! Must be at least %d\n", MIN_COLUMN_COUNT);
        return false;
    }

    int max_mines = (*row_count) * (*col_count) - 1;
    printf("Enter number of mines (1-%d): ", max_mines);
    if (scanf("%d", mine_count) != 1 || *mine_count < 1 || *mine_count > max_mines) {
        fprintf(stderr, "Invalid mine count! Must be between 1 and %d\n", max_mines);
        return false;
    }
    
    return true;
}

Board *create_our_board(int first_click_row, int first_click_column) {
    Board *board = (Board *)calloc(1, sizeof(Board));
    if (!board) return NULL;
    
    if (!read_board_parameters(&board->row_count, &board->column_count, &board->mine_count)) {
        free(board);
        return NULL;
    }

    // Выделяем память для двумерного массива
    board->tiles = (Tile **)malloc(board->row_count * sizeof(Tile *));
    for (int row = 0; row < board->row_count; row++) {
        board->tiles[row] = (Tile *)calloc(board->column_count, sizeof(Tile));
        if (!board->tiles[row]) {
            // Cleanup если выделение не удалось
            for (int r = 0; r < row; r++) free(board->tiles[r]);
            free(board->tiles);
            free(board);
            return NULL;
        }
    }

    set_mines_randomly(board, first_click_row, first_click_column);
    set_tile_values(board);
    return board;
}

// Не забудьте обновить destroy_board() для работы с динамическим массивом
void destroy_board(Board *board) {
    if (!board) return;
    
    if (board->tiles) {
        for (int row = 0; row < board->row_count; row++) {
            free(board->tiles[row]);
        }
        free(board->tiles);
    }
    free(board);
}
bool is_game_solved(Board *board) {
    assert(board != NULL);
    for (int row = 0; row < board->row_count; row++) {
        for (int column = 0; column < board->column_count; column++) {
            if (board->tiles[row][column]->tile_state == CLOSED
                && !board->tiles[row][column]->is_mine) {
                return false;
            }
        }
    }
    mark_all_mines(board);
    return true;
}

bool is_input_data_correct(Board *board, int input_row, int input_column) {
    assert(board != NULL);
    return input_row >= 0 && board->row_count > input_row
           && input_column >= 0 && board->column_count > input_column;
}

void open_all_mines(Board *board) {
    assert(board != NULL);
    for (int row = 0; row < board->row_count; row++) {
        for (int column = 0; column < board->column_count; column++) {
            if (board->tiles[row][column]->tile_state == CLOSED
                && board->tiles[row][column]->is_mine) {
                board->tiles[row][column]->tile_state = OPEN;
            }
        }
    }
}
