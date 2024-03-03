#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void draw_blocks(Vector2 *segms, int score, Vector2 cube_size, Vector2 food_pos) {
  for (int i = 0; i <= score; ++i) {
    Vector2 cube_pos = Vector2Multiply(segms[i], cube_size);
    DrawRectangleV(cube_pos, cube_size, GREEN);
  }
  DrawRectangleV(Vector2Multiply(food_pos, cube_size), cube_size, RED);
}

void get_input(Vector2 *dir) {
  if (IsKeyDown(KEY_W) && dir->y != 1) {
    *dir = (Vector2){0, -1};
  } else if (IsKeyDown(KEY_A) && dir->x != 1) {
    *dir = (Vector2){-1, 0};
  } else if (IsKeyDown(KEY_S) && dir->y != -1) {
    *dir = (Vector2){0, 1};
  } else if (IsKeyDown(KEY_D) && dir->x != -1) {
    *dir = (Vector2){1, 0};
  }
}

void place_food(Vector2 *pos, Vector2 blocks, Vector2 *segms, int score) {
  *pos = (Vector2){GetRandomValue(0, blocks.x - 1), GetRandomValue(0, blocks.y - 1)};
}

int main(void) {
  int block_side = 50;
  Vector2 block_size = {block_side, block_side};
  Vector2 blocks = {20, 15};
  Vector2 window = Vector2Multiply(blocks, block_size);
  int update_fr = 15;
  Vector2 food_pos = {-1, -1};
  Vector2 dir = {1, 0};
  Vector2 head = {0, 0};
  int score = 1;
  int segms_size = 256;
  Vector2 segms[segms_size];
  segms[0] = head;

  InitWindow(window.x, window.y, "snake game");
  SetRandomSeed(69420);
  SetTargetFPS(60);

  place_food(&food_pos, blocks, segms, score);

  int update = 0;
  while (!WindowShouldClose()) {
    if (update > update_fr) {
      fprintf(stderr, "score: %i\n", score);
      fprintf(stderr, "%.0f, %.0f\n", food_pos.x, food_pos.y);
      update = 0;
      head = Vector2Add(dir, head);
      if (Vector2Equals(head, food_pos)) {
        place_food(&food_pos, blocks, segms, score);
        ++score;
      }
      for (int i = score; i >= 0; --i) {
        segms[i] = segms[i - 1];
      }
      segms[0] = head;
    }
    ++update;
    get_input(&dir);
    BeginDrawing();
    ClearBackground(BLACK);
    draw_blocks(segms, score, block_size, food_pos);
    EndDrawing();
  }

  CloseWindow();
  return EXIT_SUCCESS;
}
