#include "raylib.h"


#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define GRID_WIDTH 32
#define GRID_HEIGHT 32

#define TOTAL_PIXELS (GRID_WIDTH * GRID_HEIGHT)
int pixels[TOTAL_PIXELS];

int pixel_size;
int pixel_size_full;

int offset_left;
int offset_right;
int offset_top;
int offset_bottom;

int export_mode = 0;

void setup(void);
void unsetup(void);

int functional(void);
void draw(void);


int main(void) {
  setup();
  while (!WindowShouldClose()) {
    if (functional() == 1) break;
    draw();
  }
  unsetup();
  return 0;
}


void setup(void) {
  InitWindow(1250, 1250, "meowpixel v0.3 beta");
  SetTargetFPS(60);

  pixel_size = 1000 / MAX(GRID_WIDTH, GRID_HEIGHT);
  pixel_size_full = MAX(GetScreenWidth(), GetScreenHeight()) / MAX(GRID_WIDTH, GRID_HEIGHT);

  offset_left = (GetScreenWidth() - (GRID_WIDTH * pixel_size)) / 2;
  offset_right = offset_left + (GRID_WIDTH * pixel_size);
  offset_top = (GetScreenHeight() - (GRID_HEIGHT * pixel_size)) / 2;
  offset_bottom = offset_top + (GRID_HEIGHT * pixel_size);
}

void unsetup(void) {
  CloseWindow();
}


int functional(void) {
  if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), (Rectangle){225, 0, MeasureText("Export Mode", 16), 20})) || (IsKeyPressed(KEY_F1))) {
    export_mode = !export_mode;
  } else if (((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), (Rectangle){350, 0, MeasureText("Clear Screen", 16), 20})) || IsKeyPressed(KEY_SPACE))) {
     for (int i = 0; i < sizeof(pixels) / sizeof(pixels[0]); i++)
       pixels[i] = 0;
  } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), (Rectangle){GetScreenWidth() - MeasureText("Exit", 16) - 5, 0, MeasureText("Exit", 16), 20})) {
    return 1;
  } else if (!export_mode && (IsMouseButtonDown(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_RIGHT))) {
    int click_value = IsMouseButtonDown(MOUSE_BUTTON_LEFT) ? 1 : 0;

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
  } else if (export_mode && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), (Rectangle){0, 20, GetScreenWidth(), GetScreenHeight() - 20})) {
    Image img = GenImageColor(GRID_WIDTH * 10, GRID_HEIGHT * 10, BLANK);

    for (int i = 0; i < TOTAL_PIXELS; i++) {
      int col = i % GRID_WIDTH;
      int row = i / GRID_WIDTH;

      Color clr = pixels[i] ? BLACK : WHITE;
      for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
          int x = (col * 10) + j;
          int y = (row * 10) + i;

          ImageDrawPixel(&img, x, y, clr);
        }
      }
    }

    ExportImage(img, "meowpixel_test_export_image.png");
    UnloadImage(img);
  }

  return 0;
}


void draw(void) {
  BeginDrawing();
    if (!export_mode) {
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

      DrawText("LMB to draw", GetScreenWidth() - MeasureText("LMB to draw", 16) - 5, GetScreenHeight() - 16 * 5 - 5, 16, BLACK);
      DrawText("RMB to erase", GetScreenWidth() - MeasureText("RMB to erase", 16) - 5, GetScreenHeight() - 16 * 4 - 5, 16, BLACK);
      DrawText("Space to clear screen", GetScreenWidth() - MeasureText("Space to clear screen", 16) - 5, GetScreenHeight() - 16 * 3 - 5, 16, BLACK);
      DrawText("F1 to toggle export mode", GetScreenWidth() - MeasureText("F1 to toggle export mode", 16) - 5, GetScreenHeight() - 16 * 1 - 5, 16, BLACK);

      DrawFPS(5, GetScreenHeight() - 21);
    } else if (export_mode) {
      ClearBackground((Color){255, 255, 255, 255});

      for (int i = 0; i < TOTAL_PIXELS; i++) {
        int col = i % GRID_WIDTH;
        int row = i / GRID_WIDTH;

        int posX = col * pixel_size_full;
        int posY = row * pixel_size_full;

        DrawRectangle(posX, posY, pixel_size_full, pixel_size_full, pixels[i] ? BLACK : WHITE);
      }
    }

    DrawRectangle(0, 0, GetScreenWidth(), 20, WHITE);
    DrawText("meowpixel v0.3 beta", 5, 2, 16, BLACK);
    DrawText("Export Mode", 225, 2, 16, export_mode ? SKYBLUE : BLACK);
    DrawText("|", 336, 2, 16, BLACK);
    DrawText("Clear Screen", 350, 2, 16, BLACK);
    DrawText("Exit", GetScreenWidth() - MeasureText("Exit", 16) - 5, 2, 16, RED);
    DrawLine(0, 20, GetScreenWidth(), 20, BLACK);
  EndDrawing();
}
