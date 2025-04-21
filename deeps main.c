#include <stdlib.h>
#include <stdio.h>
#include "game.h"
#include "ui.h"
#include "board.h"

int main() {
    Game *game = create_game();
    if (!game) {
        fprintf(stderr, "Failed to create game\n");
        return EXIT_FAILURE;
    }
    
    read_player_name(game);
    
    Board *board = create_our_board();
    if (!board) {
        fprintf(stderr, "Failed to create game board\n");
        destroy_game(game);
        return EXIT_FAILURE;
    }
    
    game->board = board;
    play_game(game);
    destroy_game(game);
    dialog();
    return EXIT_SUCCESS;
}
