#include <raylib.h>

int main() {
  InitWindow(600, 600, "First raylib window");
  SetTargetFPS(120);

  int x = 0, y = 0;

  // TODO: Make snake game
  // It will be easy. Right? RIGHT?
  while (!WindowShouldClose()) {
    if (IsKeyDown(KEY_D))
      x++;
    if (IsKeyDown(KEY_A))
      x--;
    if (IsKeyDown(KEY_S))
      y++;
    if (IsKeyDown(KEY_W))
      y--;

    BeginDrawing();
    ClearBackground(WHITE);
    DrawRectangle(x, y, 10, 10, BLACK);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
