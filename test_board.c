#include "greatest.h"
#include "../board.h"
#include "../game.h"

TEST verify_with_high_coordinates() {
    Board *board = create_board(10, 10, 5);
    ASSERT(board != NULL);
    ASSERT_FALSE(is_input_data_correct(board, 6, 11));
    destroy_board(board);
    PASS();
}

TEST verify_with_low_coordinates() {
    Board *board = create_board(10, 10, 5);
    ASSERT(board != NULL);
    ASSERT_FALSE(is_input_data_correct(board, -1, 4));
    destroy_board(board);
    PASS();
}

TEST verify_with_correct_coordinates() {
    Board *board = create_board(10, 10, 5);
    ASSERT(board != NULL);
    ASSERT(is_input_data_correct(board, 0, 9));
    destroy_board(board);
    PASS();
}

TEST is_game_solved_at_the_start() {
    Game *game = create_game();
    Board *board = create_board(10, 10, 5);
    ASSERT(board != NULL);
    set_mines_randomly(board, 0, 0);
    game->board = board;

    bool is_solved = is_game_solved(game->board);
    ASSERT_FALSE(is_solved);
    destroy_game(game);
    PASS();
}

TEST is_game_solved_if_solved() {
    Game *game = create_game();
    Board *board = create_board(10, 10, 5);
    ASSERT(board != NULL);
    set_mines_randomly(board, 0, 0);
    set_tile_values(board);
    game->board = board;

    for (int row = 0; row < game->board->row_count; row++) {
        for (int column = 0; column < game->board->column_count; column++) {
            if (!game->board->tiles[row][column]->is_mine) {
                game->board->tiles[row][column]->tile_state = OPEN;
            }
        }
    }
    ASSERT(is_game_solved(game->board));
    destroy_game(game);
    PASS();
}

TEST open_mines_after_loss() {
    Game *game = create_game();
    Board *board = create_board(10, 10, 5);
    ASSERT(board != NULL);
    set_mines_randomly(board, 0, 0);
    set_tile_values(board);
    game->board = board;

    for (int row = 0; row < game->board->row_count; row++) {
        for (int column = 0; column < game->board->column_count; column++) {
            if (game->board->tiles[row][column]->is_mine) {
                game->board->tiles[row][column]->tile_state = OPEN;
                break;
            }
        }
    }
    open_all_mines(game->board);
    for (int row = 0; row < game->board->row_count; row++) {
        for (int column = 0; column < board->column_count; column++) {
            if (game->board->tiles[row][column]->is_mine) {
                ASSERT_EQ(OPEN, game->board->tiles[row][column]->tile_state);
            }
        }
    }
    destroy_game(game);
    PASS();
}

TEST is_mine_on_returns_true_for_mine() {
    Board *board = create_board(5, 5, 1);
    ASSERT(board != NULL);
    board->tiles[2][3]->is_mine = true;

    ASSERT(is_mine_on(board, 2, 3));
    destroy_board(board);
    PASS();
}

TEST is_mine_on_returns_false_for_non_mine() {
    Board *board = create_board(5, 5, 1);
    ASSERT(board != NULL);
    ASSERT_FALSE(is_mine_on(board, 1, 1));
    destroy_board(board);
    PASS();
}

TEST is_mine_on_out_of_bounds_returns_false() {
    Board *board = create_board(5, 5, 1);
    ASSERT(board != NULL);
    ASSERT_FALSE(is_mine_on(board, -1, 0));
    ASSERT_FALSE(is_mine_on(board, 0, 5));
    ASSERT_FALSE(is_mine_on(board, 5, 5));
    destroy_board(board);
    PASS();
}

TEST count_neighbour_mines_with_single_mine_nearby() {
    Board *board = create_board(3, 3, 1);
    ASSERT(board != NULL);
    board->tiles[0][0]->is_mine = true;
    int count = count_neighbour_mines(board, 1, 1);
    ASSERT_EQ(1, count);
    destroy_board(board);
    PASS();
}

TEST set_tile_values_sets_correct_values() {
    Board *board = create_board(3, 3, 1);
    ASSERT(board != NULL);
    board->tiles[0][0]->is_mine = true;
    set_tile_values(board);
    ASSERT_EQ(-1, board->tiles[0][0]->value);
    ASSERT_EQ(1, board->tiles[0][1]->value);
    ASSERT_EQ(1, board->tiles[1][0]->value);
    ASSERT_EQ(1, board->tiles[1][1]->value);
    destroy_board(board);
    PASS();
}

TEST mark_all_mines_marks_closed_tiles() {
    Board *board = create_board(3, 3, 1);
    ASSERT(board != NULL);
    board->tiles[0][0]->tile_state = CLOSED;
    board->tiles[0][1]->tile_state = OPEN;
    mark_all_mines(board);
    ASSERT_EQ(MARKED, board->tiles[0][0]->tile_state);
    ASSERT_EQ(OPEN, board->tiles[0][1]->tile_state);
    destroy_board(board);
    PASS();
}

TEST generate_random_coordinates_within_range() {
    srand(0);
    for (int i = 0; i < 100; i++) {
        int result = generate_random_coordinates(10);
        ASSERT(result >= 0 && result < 10);
    }
    PASS();
}

TEST set_mines_randomly_sets_correct_mine_count() {
    Board *board = create_board(5, 5, 5);
    ASSERT(board != NULL);
    srand(0);
    set_mines_randomly(board, 2, 2);
    int mine_count = 0;

    for (int row = 0; row < board->row_count; row++) {
        for (int col = 0; col < board->column_count; col++) {
            if (board->tiles[row][col]->is_mine) {
                mine_count++;
                ASSERT_FALSE(row == 2 && col == 2);
            }
        }
    }
    ASSERT_EQ(5, mine_count);
    destroy_board(board);
    PASS();
}

TEST set_mines_randomly_skips_already_mined() {
    Board *board = create_board(2, 2, 1);
    ASSERT(board != NULL);
    board->tiles[0][0]->is_mine = true;
    board->tiles[0][1]->is_mine = true;
    srand(0);
    set_mines_randomly(board, 1, 1);
    int mine_count = 0;

    for (int row = 0; row < board->row_count; row++) {
        for (int col = 0; col < board->column_count; col++) {
            if (board->tiles[row][col]->is_mine) {
                mine_count++;
            }
        }
    }
    ASSERT_EQ(3, mine_count);
    destroy_board(board);
    PASS();
}

TEST create_board_invalid_parameters() {
    Board *board = create_board(31, 5, 5);
    ASSERT(board == NULL);
    PASS();
}

TEST create_interactive_board_valid_parameters() {
    FILE *input_file = fopen("test_input.txt", "w");
    ASSERT(input_file != NULL);
    fprintf(input_file, "5\n5\n5\n");
    fclose(input_file);

    input_file = fopen("test_input.txt", "r");
    ASSERT(input_file != NULL);
    FILE *original_stdin = stdin;
    stdin = input_file;
    Board *board = create_interactive_board();
    stdin = original_stdin;
    fclose(input_file);
    remove("test_input.txt");

    ASSERT(board != NULL);
    if (board != NULL) {
        ASSERT_EQ(5, board->row_count);
        ASSERT_EQ(5, board->column_count);
        ASSERT_EQ(5, board->mine_count);
        int area_field = board->row_count * board->column_count;
        ASSERT(board->mine_count < area_field);

        for (int row = 0; row < board->row_count; row++) {
            for (int column = 0; column < board->column_count; column++) {
                ASSERT(board->tiles[row][column]->tile_state == CLOSED);
                ASSERT_FALSE(board->tiles[row][column]->is_mine);
            }
        }
        destroy_board(board);
    }
    PASS();
}

TEST create_interactive_board_invalid_row_count() {
    FILE *input_file = fopen("test_input.txt", "w");
    ASSERT(input_file != NULL);
    fprintf(input_file, "-1\n5\n5\n");
    fclose(input_file);

    input_file = fopen("test_input.txt", "r");
    ASSERT(input_file != NULL);
    FILE *original_stdin = stdin;
    stdin = input_file;
    Board *board = create_interactive_board();
    stdin = original_stdin;
    fclose(input_file);
    remove("test_input.txt");

    ASSERT(board == NULL);
    PASS();
}

TEST create_interactive_board_invalid_col_count() {
    FILE *input_file = fopen("test_input.txt", "w");
    ASSERT(input_file != NULL);
    fprintf(input_file, "5\n-1\n5\n");
    fclose(input_file);

    input_file = fopen("test_input.txt", "r");
    ASSERT(input_file != NULL);
    FILE *original_stdin = stdin;
    stdin = input_file;
    Board *board = create_interactive_board();
    stdin = original_stdin;
    fclose(input_file);
    remove("test_input.txt");

    ASSERT(board == NULL);
    PASS();
}

TEST create_interactive_board_invalid_mine_count() {
    FILE *input_file = fopen("test_input.txt", "w");
    ASSERT(input_file != NULL);
    fprintf(input_file, "5\n5\n0\n");
    fclose(input_file);

    input_file = fopen("test_input.txt", "r");
    ASSERT(input_file != NULL);
    FILE *original_stdin = stdin;
    stdin = input_file;
    Board *board = create_interactive_board();
    stdin = original_stdin;
    fclose(input_file);
    remove("test_input.txt");

    ASSERT(board == NULL);
    PASS();
}

TEST create_interactive_board_invalid_mine_count_too_large() {
    FILE *input_file = fopen("test_input.txt", "w");
    ASSERT(input_file != NULL);
    fprintf(input_file, "5\n5\n25\n");
    fclose(input_file);

    input_file = fopen("test_input.txt", "r");
    ASSERT(input_file != NULL);
    FILE *original_stdin = stdin;
    stdin = input_file;
    Board *board = create_interactive_board();
    stdin = original_stdin;
    fclose(input_file);
    remove("test_input.txt");

    ASSERT(board == NULL);
    PASS();
}

SUITE(test_board) {
    RUN_TEST(verify_with_high_coordinates);
    RUN_TEST(verify_with_low_coordinates);
    RUN_TEST(verify_with_correct_coordinates);
    RUN_TEST(is_game_solved_at_the_start);
    RUN_TEST(is_game_solved_if_solved);
    RUN_TEST(open_mines_after_loss);
    RUN_TEST(is_mine_on_returns_true_for_mine);
    RUN_TEST(is_mine_on_returns_false_for_non_mine);
    RUN_TEST(is_mine_on_out_of_bounds_returns_false);
    RUN_TEST(count_neighbour_mines_with_single_mine_nearby);
    RUN_TEST(set_tile_values_sets_correct_values);
    RUN_TEST(mark_all_mines_marks_closed_tiles);
    RUN_TEST(generate_random_coordinates_within_range);
    RUN_TEST(set_mines_randomly_sets_correct_mine_count);
    RUN_TEST(set_mines_randomly_skips_already_mined);
    RUN_TEST(create_board_invalid_parameters);
    RUN_TEST(create_interactive_board_valid_parameters);
    RUN_TEST(create_interactive_board_invalid_row_count);
    RUN_TEST(create_interactive_board_invalid_col_count);
    RUN_TEST(create_interactive_board_invalid_mine_count);
    RUN_TEST(create_interactive_board_invalid_mine_count_too_large);
}
