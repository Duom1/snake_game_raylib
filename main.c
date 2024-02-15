#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t direction_t;

enum Block_type {
  NONE = 0,
  SNAKE = 1,
  FOOD,
};

enum Direction_type {
  UP = 1,
  DOWN,
  LEFT,
  RIGHT,
};

void move_snake(direction_t dir, int *head, int *tail, int new_tail,
                uint8_t *board, int row_size) {
  board[*tail] = NONE;
  *tail = new_tail;
  if (dir == UP) {
    *head = *head - row_size;
    board[*head] = SNAKE;
  } else if (dir == DOWN) {
    *head = *head + row_size;
    board[*head] = SNAKE;
  } else if (dir == LEFT) {
    *head = *head - 1;
    board[*head] = SNAKE;
  } else if (dir == RIGHT) {
    *head = *head + 1;
    board[*head] = SNAKE;
  }
}

direction_t get_input(direction_t cur) {
  if (IsKeyPressed(KEY_UP)) {
    return UP;
  } else if (IsKeyPressed(KEY_DOWN)) {
    return DOWN;
  } else if (IsKeyPressed(KEY_LEFT)) {
    return LEFT;
  } else if (IsKeyPressed(KEY_RIGHT)) {
    return RIGHT;
  }
  return cur;
}

int place_random_food(uint8_t *board, int board_size) {
  int ans;
  while (true) {
    ans = GetRandomValue(0, board_size - 1);
    if (board[ans] != SNAKE) {
      board[ans] = FOOD;
      break;
    }
  }
  return ans;
}

void display_board(uint8_t *board, int board_size, int block_size,
                   int blocks_per_row) {
  for (int i = 0; i <= board_size - 1; ++i) {
    Color color;
    double roundness;
    switch (board[i]) {
    case SNAKE:
      color = GREEN;
      roundness = .7f;
      break;
    case FOOD:
      color = RED;
      roundness = 1.0f;
      break;
    default:
      continue;
    }
    Rectangle rect = {0, 0, 0, 0};
    rect.x = (i % blocks_per_row) * block_size;
    rect.y = (int)(i / blocks_per_row) * block_size;
    rect.width = block_size;
    rect.height = block_size;
    DrawRectangleRounded(rect, roundness, 5, color);
  }
}

int main(int argc, char **argv) {
  if (argc < 5 || argc > 5) {
    fprintf(stderr,
            "too many or few arguments\nusage:\n\t%s <blocks x> <blocks y> "
            "<game speed> <snake length> <block size in pixels>\n",
            argv[0]);
    exit(EXIT_FAILURE);
  }

  int block_size = atoi(argv[4]);
  int speed = atoi(argv[3]);
  int speed_mod = 0;
  int game_x = atoi(argv[1]);
  int game_y = atoi(argv[2]);
  int wx = game_x * block_size;
  int wy = game_y * block_size;
  int board_size = game_x * game_y;
  int player_head = board_size / 2 - 1;
  int player_tail = player_head;
  int text_size = (int)wy * 0.06;
  uint8_t *board = malloc(board_size * sizeof(uint8_t));
  direction_t dir = RIGHT;

  board[player_head] = SNAKE;
  place_random_food(board, board_size);

  // for printing info about the starting values
#ifndef NDEBUG
  printf("blocks: %i x %i, speed: %i, block size: %i, window: %i x %i, board "
         "size: %i, text size: %i\n",
         game_x, game_y, speed, block_size, wx, wy, board_size, text_size);
#endif

  InitWindow(wx, wy, "snake game");
  SetRandomSeed(42069);
  SetTargetFPS(60);

  bool playing = false;
  // GAME LOOP
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
    dir = get_input(dir);
    if (speed_mod > speed) {
      speed_mod = 0;
      move_snake(dir, &player_head, &player_tail, player_head, board, game_x);
    }
    ++speed_mod;

    // rendering
    BeginDrawing();
    ClearBackground(BLACK);
    display_board(board, board_size, block_size, game_x);
    EndDrawing();
  } // end of GAME LOOP

quit_game:
  printf("-->%p\n", board);
  free(board);
  CloseWindow();
  return EXIT_SUCCESS;
}
