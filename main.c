#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void draw_blocks(Vector2 *segms, int score, Vector2 cube_size,
                 Vector2 food_pos) {
  for (int i = 0; i <= score; ++i) {
    Vector2 cube_pos = Vector2Multiply(segms[i], cube_size);
    DrawRectangleV(cube_pos, cube_size, GREEN);
  }
  DrawRectangleV(Vector2Multiply(food_pos, cube_size), cube_size, RED);
}

void get_input(Vector2 *dir, Vector2 *coming, bool *paused) {
  if (IsKeyPressed(KEY_ESCAPE)) {
    *paused = !*paused;
  } else if (IsKeyDown(KEY_W) && coming->y != 1) {
    *dir = (Vector2){0, -1};
  } else if (IsKeyDown(KEY_A) && coming->x != 1) {
    *dir = (Vector2){-1, 0};
  } else if (IsKeyDown(KEY_S) && coming->y != -1) {
    *dir = (Vector2){0, 1};
  } else if (IsKeyDown(KEY_D) && coming->x != -1) {
    *dir = (Vector2){1, 0};
  }
}

void place_food(Vector2 *pos, Vector2 blocks, Vector2 *segms, int score) {
  *pos = (Vector2){GetRandomValue(0, blocks.x - 1),
                   GetRandomValue(0, blocks.y - 1)};
  for (int i = 0; i <= score; ++i) {
    if (Vector2Equals(segms[i], *pos)) {
      place_food(pos, blocks, segms, score);
    }
  }
}

bool self_collision_check(Vector2 *segms, int score) {
  for (int i = 1; i <= score; ++i) {
    if (Vector2Equals(segms[i], segms[0])) {
      return true;
    }
  }
  return false;
}

bool out_of_bounds(Vector2 head, Vector2 bounds) {
  if (FloatEquals(head.x, -1.0) || FloatEquals(head.y, -1.0) ||
      FloatEquals(head.x, bounds.x) || FloatEquals(head.y, bounds.y)) {
    return true;
  }
  return false;
}

void update_snake(Vector2 *segms, Vector2 head, int score) {
  for (int i = score; i >= 0; --i) {
    segms[i] = segms[i - 1];
  }
  segms[0] = head;
}

void game_over(int score) {
  char str_score[32];
  sprintf(str_score, "score: %i", score);
  for (int i = 0; i < 100; ++i) {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("GAME OVER!", 10, 10, 120, RED);
    DrawText(str_score, 10, 140, 120, RED);
    EndDrawing();
  }
}

void draw_score(int score) {
  char str_score[32];
  sprintf(str_score, "score: %i", score);
  DrawText(str_score, 10, 10, 40, BLUE);
}

int main(void) {
  int block_side = 50;
  Vector2 block_size = {block_side, block_side};
  Vector2 blocks = {20, 15};
  Vector2 window = Vector2Multiply(blocks, block_size);
  int update_fr = 15;
  Vector2 food_pos = {-1, -1};
  Vector2 dir = {1, 0};
  Vector2 coming = dir;
  Vector2 head = {0, 0};
  int score = 0;
  int segms_size = 256;
  Vector2 segms[segms_size];
  segms[0] = head;

  InitWindow(window.x, window.y, "snake game");
  SetExitKey(KEY_NULL);
  SetTargetFPS(60);

  place_food(&food_pos, blocks, segms, score);

  bool paused = false;
  int update = 0;
  while (!WindowShouldClose()) {
    // updating and getting input
    if (update > update_fr && !paused) {
      update = 0;
      head = Vector2Add(dir, head);
      if (out_of_bounds(head, blocks) || self_collision_check(segms, score)) {
        game_over(score);
        score = 0;
        head = (Vector2){0, 0};
        dir = (Vector2){1, 0};
        place_food(&food_pos, blocks, segms, score);
      }
      if (Vector2Equals(head, food_pos)) {
        place_food(&food_pos, blocks, segms, score);
        ++score;
      }
      update_snake(segms, head, score);
      coming = dir;
    }
    ++update;
    get_input(&dir, &coming, &paused);
    // rendering
    BeginDrawing();
    ClearBackground(BLACK);
    draw_blocks(segms, score, block_size, food_pos);
    if (paused) {
      DrawText("Paused", 10, 10, 100, RED);
    } else {
      draw_score(score);
    }
    EndDrawing();
  }

  CloseWindow();
  return EXIT_SUCCESS;
}
