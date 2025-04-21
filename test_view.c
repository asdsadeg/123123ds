#include "greatest.h"
#include "../board.h"
#include "../view.h"

TEST view_play_field_empty_board() {
    Board *board = create_board(3, 3, 1);
    ASSERT(board != NULL);
    char *result = view_play_field(board, 1, 1);
    ASSERT_STR_EQ("   1 2 3 \n1  - - - \n2  - - - \n3  - - - \n", result);
    free(result);
    destroy_board(board);
    PASS();
}

TEST view_play_field_with_open_tile() {
    Board *board = create_board(3, 3, 1);
    ASSERT(board != NULL);
    board->tiles[1][1]->tile_state = OPEN;
    board->tiles[1][1]->value = 0;
    char *result = view_play_field(board, 1, 1);
    ASSERT_STR_EQ("   1 2 3 \n1  - - - \n2  - 0 - \n3  - - - \n", result);
    free(result);
    destroy_board(board);
    PASS();
}

TEST view_play_field_with_marked_tile() {
    Board *board = create_board(3, 3, 1);
    ASSERT(board != NULL);
    board->tiles[1][1]->tile_state = MARKED;
    char *result = view_play_field(board, 1, 1);
    ASSERT_STR_EQ("   1 2 3 \n1  - - - \n2  - ! - \n3  - - - \n", result);
    free(result);
    destroy_board(board);
    PASS();
}

TEST view_play_field_with_mine() {
    Board *board = create_board(3, 3, 1);
    ASSERT(board != NULL);
    board->tiles[1][1]->is_mine = true;
    board->tiles[1][1]->tile_state = OPEN;
    char *result = view_play_field(board, 1, 1);

    ASSERT_STR_EQ("   1 2 3 \n1  - - - \n2  - X - \n3  - - - \n", result);
    free(result);
    destroy_board(board);
    PASS();
}

TEST view_play_field_invalid_board() {
    Board *board = create_board(31, 5, 5);
    char *result = view_play_field(board, 1, 1);
    ASSERT_STR_EQ("Board is NULL\n", result);
    free(result);
    PASS();
}

TEST view_play_field_null_board() {
    char *result = view_play_field(NULL, 1, 1);
    ASSERT_STR_EQ("Board is NULL\n", result);
    free(result);
    PASS();
}

SUITE(test_view) {
    RUN_TEST(view_play_field_empty_board);
    RUN_TEST(view_play_field_with_open_tile);
    RUN_TEST(view_play_field_with_marked_tile);
    RUN_TEST(view_play_field_with_mine);
    RUN_TEST(view_play_field_invalid_board);
    RUN_TEST(view_play_field_null_board);
}
