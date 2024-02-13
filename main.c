#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum Block_type {
  SNAKE = 1,
  FOOD,
};

void displayBoard(uint8_t *board, int board_size, int block_size,
                  int blocks_per_row) {
  for (int i = 0; i <= board_size - 1; ++i) {
    Color color;
    switch (board[i]) {
    case SNAKE:
      color = GREEN;
      break;
    case FOOD:
      color = RED;
      break;
    default:
      continue;
    }
    Rectangle rect = {0, 0, 0, 0};
    rect.x = (i % blocks_per_row) * block_size;
    rect.y = (int)(i / blocks_per_row) * block_size;
    rect.width = block_size;
    rect.height = block_size;
    DrawRectangleRounded(rect, .4f, 5, color);
  }
}

int main(int argc, char **argv) {
  if (argc < 6 || argc > 6) {
    fprintf(stderr,
            "too many or few arguments\nusage:\n\t%s <blocks x> <blocks y> "
            "<game speed> <snake length> <block size in pixels>\n",
            argv[0]);
    exit(EXIT_FAILURE);
  }

  int speed, player_len, game_x, game_y, wx, wy, block_size, text_size,
      board_size;

  block_size = atoi(argv[5]);
  player_len = atoi(argv[4]);
  speed = atoi(argv[3]);
  game_x = atoi(argv[1]);
  game_y = atoi(argv[2]);
  wx = game_x * block_size;
  wy = game_y * block_size;
  board_size = game_x * game_y;
  text_size = (int)wy * 0.06;
  uint8_t *board = malloc(board_size * sizeof(uint8_t));

  board[0] = FOOD;
  board[29] = FOOD;
  board[board_size - 1] = SNAKE;

  // for printing info about the starting values
#ifndef NDEBUG
  printf("arguments: %i, %i, %i, %i, %i\n", game_x, game_y, speed, player_len,
         block_size);
#endif

  InitWindow(wx, wy, "snake game");
  SetTargetFPS(60);

  bool playing = false;
  while (!WindowShouldClose()) {
    // loop 1 for starting game
    while (!playing) {
      if (WindowShouldClose()) {
        goto quit_game;
      }
      if (IsKeyPressed(KEY_SPACE)) {
        playing = true;
        break;
      }
      BeginDrawing();
      ClearBackground(BLACK);
      DrawText("press space to play", 10, 10, text_size, WHITE);
      EndDrawing();
    } // end of loop 1
    // game logic

    // rendering
    BeginDrawing();
    ClearBackground(BLACK);
    displayBoard(board, board_size, block_size, game_x);
    EndDrawing();
  }

quit_game:
  free(board);
  CloseWindow();
  return EXIT_SUCCESS;
}
