#include "raylib.h"


#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define GRID_WIDTH 32
#define GRID_HEIGHT 32

#define TOTAL_PIXELS (GRID_WIDTH * GRID_HEIGHT)
int pixels[TOTAL_PIXELS];

int pixel_size;

int offset_left;
int offset_right;
int offset_top;
int offset_bottom;

void setup(void);
void unsetup(void);

void functional(void);
void draw(void);


int main(void) {
  setup();
  while (!WindowShouldClose()) {
    functional();
    draw();
  }
  unsetup();
  return 0;
}


void setup(void) {
  InitWindow(1250, 1250, "meowpixel v0.1 beta");
  SetTargetFPS(60);

  pixel_size = 1000 / MAX(GRID_WIDTH, GRID_HEIGHT);

  offset_left = (GetScreenWidth() - (GRID_WIDTH * pixel_size)) / 2;
  offset_right = offset_left + (GRID_WIDTH * pixel_size);
  offset_top = (GetScreenHeight() - (GRID_HEIGHT * pixel_size)) / 2;
  offset_bottom = offset_top + (GRID_HEIGHT * pixel_size);
}

void unsetup(void) {
  CloseWindow();
}


void functional(void) {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
    int click_value = IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ? 1 : 0;

    for (int i = 0; i < TOTAL_PIXELS; i++) {
      int col = i % GRID_WIDTH;
      int row = i / GRID_WIDTH;

      int posX = offset_left + (col * pixel_size);
      int posY = offset_top + (row * pixel_size);

      if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){posX, posY, pixel_size, pixel_size})) {
        pixels[i] = click_value;
        break;
      }
    }
  }
}


void draw(void) {
  BeginDrawing();
    ClearBackground((Color){255, 255, 245, 255});

    for (int i = 0; i < TOTAL_PIXELS; i++) {
      int col = i % GRID_WIDTH;
      int row = i / GRID_WIDTH;

      int posX = offset_left + (col * pixel_size);
      int posY = offset_top + (row * pixel_size);

      DrawRectangle(posX, posY, pixel_size, pixel_size, pixels[i] ? BLACK : WHITE);
    }

    for (int i = offset_left; i <= offset_right; i += pixel_size)
        DrawLine(i, offset_top, i, offset_bottom, BLACK);

    for (int i = offset_top; i <= offset_bottom; i += pixel_size)
        DrawLine(offset_left, i, offset_right, i, BLACK);

    DrawFPS(0, 0);
  EndDrawing();
}
