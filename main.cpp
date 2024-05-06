#include <array>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <raylib.h>
#include <string>
#include <vector>

const int windowSize = 600;
const int cells = 15;

class Cell {
public:
  int x, y;
  const static int cellSize = windowSize / cells;

  Cell(int _x, int _y) {
    x = _x;
    y = _y;
  }

  Cell() {}
};

class Snake {
public:
  Cell head = Cell(0, 0), &tail = head;
  std::vector<Cell> body;
  std::array<int, 2> d = {1, 0};

  void grow() {
    Cell newCell(tail.x, tail.y);
    tail = newCell;
    body.push_back(newCell);
  }

  bool move() {
    for (int i = body.size() - 1; i >= 0; i--) {
      int nextX, nextY;
      if (i > 0) {
        nextX = body[i - 1].x;
        nextY = body[i - 1].y;
      } else {
        nextX = head.x;
        nextY = head.y;
      }

      body[i].x = nextX;
      body[i].y = nextY;
    }

    head.x += d[0] * Cell::cellSize;
    head.y += d[1] * Cell::cellSize;

    if (head.x >= windowSize)
      head.x = 0;
    if (head.y >= windowSize)
      head.y = 0;
    if (head.x < 0)
      head.x = windowSize - Cell::cellSize;
    if (head.y < 0)
      head.y = windowSize - Cell::cellSize;

    for (Cell cell : body)
      if (head.x == cell.x && head.y == cell.y)
        return true;

    return false;
  }
};

class Apple : public Cell {
private:
  void moveToRandom() {
    x = (int)((double)rand() / RAND_MAX * cells) * Cell::cellSize,
    y = (int)((double)rand() / RAND_MAX * cells) * Cell::cellSize;
  }

public:
  void eat(Snake &snake) {
    moveToRandom();
    snake.grow();
  }

  Apple() : Cell() { moveToRandom(); }
};

int main() {
  srand(time(NULL));

  // Disable exit key (escape)
  SetExitKey(0);

  InitWindow(windowSize, windowSize, "First raylib window");
  SetTargetFPS(120);

  bool running = false, gameOver = false;
  Snake snake;
  Apple apple;
  double currentTime, updateTime = 0.0;
  int score = 0;
  std::queue<std::array<int, 2>> moveQueue;

  while (!WindowShouldClose()) {
    if (!running) {
      if (gameOver) {
        if (IsKeyPressed(KEY_SPACE))
          break;

        BeginDrawing();
        std::string resultStr =
            "GAME OVER!\n\nSCORE: " + std::to_string(score) +
            "\n\n\nPress SPACE to quit";
        DrawText(resultStr.c_str(), 20, 20, 30, WHITE);
        EndDrawing();

        continue;
      }

      if (IsKeyPressed(KEY_SPACE))
        running = true;

      BeginDrawing();
      ClearBackground(BLACK);
      DrawText("Press SPACE to start", 20, 20, 30, WHITE);
      EndDrawing();

      continue;
    }

    if (snake.head.x == apple.x && snake.head.y == apple.y) {
      apple.eat(snake);
      score++;
    }

    if (IsKeyPressed(KEY_A))
      moveQueue.push({-1, 0});
    if (IsKeyPressed(KEY_D))
      moveQueue.push({1, 0});
    if (IsKeyPressed(KEY_S))
      moveQueue.push({0, 1});
    if (IsKeyPressed(KEY_W))
      moveQueue.push({0, -1});

    currentTime = GetTime();
    // Move in 10FPS
    if (currentTime - updateTime > 1.0 / 10) {
      updateTime = currentTime;

      if (!moveQueue.empty()) {
        auto newD = moveQueue.front();
        if (-newD[0] != snake.d[0] || -newD[1] != snake.d[1])
          snake.d = newD;
        moveQueue.pop();
      }

      if (snake.move()) {
        running = false;
        gameOver = true;
        continue;
      }
    }

    BeginDrawing();

    ClearBackground(BLACK);

    // Draw snake
    DrawRectangle(snake.head.x, snake.head.y, Cell::cellSize, Cell::cellSize,
                  (Color){70, 200, 70, 255});
    for (Cell cell : snake.body)
      DrawRectangle(cell.x, cell.y, Cell::cellSize, Cell::cellSize,
                    (Color){100, 255, 100, 255});

    // Draw apple
    DrawRectangle(apple.x, apple.y, Cell::cellSize, Cell::cellSize, RED);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
