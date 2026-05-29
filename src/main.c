#include "raylib.h"


#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define GRID_WIDTH 16
#define GRID_HEIGHT 16

#define TOTAL_PIXELS (GRID_WIDTH * GRID_HEIGHT)
int pixels[TOTAL_PIXELS];

int pixel_size;
int pixel_size_full;

int offset_left;
int offset_right;
int offset_top;
int offset_bottom;

Vector2 export_window_pos;

int export_mode = 0;

int size;

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
  InitWindow(1250, 1250, "meowpixel v0.4 beta");
  SetTargetFPS(60);

  pixel_size = 1000 / MAX(GRID_WIDTH, GRID_HEIGHT);
  pixel_size_full = MAX(GetScreenWidth(), GetScreenHeight()) / MAX(GRID_WIDTH, GRID_HEIGHT);

  offset_left = (GetScreenWidth() - (GRID_WIDTH * pixel_size)) / 2;
  offset_right = offset_left + (GRID_WIDTH * pixel_size);
  offset_top = (GetScreenHeight() - (GRID_HEIGHT * pixel_size)) / 2;
  offset_bottom = offset_top + (GRID_HEIGHT * pixel_size);

  export_window_pos = (Vector2){ GetScreenWidth() / 2 - 500 / 2, GetScreenHeight() / 2 - 500 / 2 };
}

void unsetup(void) {
  CloseWindow();
}


int functional(void) {
  if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), (Rectangle){225, 0, MeasureText("Export Mode", 16), 20})) || (IsKeyPressed(KEY_F1))) {
    export_mode = export_mode ? 0 : 1;
  } else if (((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), (Rectangle){350, 0, MeasureText("Clear Screen", 16), 20})) || IsKeyPressed(KEY_SPACE))) {
     for (int i = 0; i < sizeof(pixels) / sizeof(pixels[0]); i++)
       pixels[i] = 0;
  } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), (Rectangle){GetScreenWidth() - MeasureText("Exit", 16) - 5, 0, MeasureText("Exit", 16), 20})) {
    return 1;
  } else if ((IsMouseButtonDown(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) && !export_mode) {
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
  } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), (Rectangle){0, 20, GetScreenWidth(), GetScreenHeight() - 20}) && export_mode == 1) {
    export_mode = 2;
  } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), (Rectangle){export_window_pos.x + 5, (export_window_pos.y + 16 * 2) + 5, 500, 16})) {
    size = 16;
    export_mode = 3;
  } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), (Rectangle){export_window_pos.x + 5, (export_window_pos.y + 16 * 3) + 5, 500, 16})) {
    size = 32;
    export_mode = 3;
  } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), (Rectangle){export_window_pos.x + 5, (export_window_pos.y + 16 * 4) + 5, 500, 16})) {
    size = 64;
    export_mode = 3;
  } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), (Rectangle){export_window_pos.x + 5, (export_window_pos.y + 16 * 5) + 5, 500, 16})) {
    size = 128;
    export_mode = 3;
  } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), (Rectangle){export_window_pos.x + 5, (export_window_pos.y + 16 * 6) + 5, 500, 16})) {
    size = 256;
    export_mode = 3;
  } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), (Rectangle){export_window_pos.x + 5, (export_window_pos.y + 16 * 7) + 5, 500, 16})) {
    size = 512;
    export_mode = 3;
  } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), (Rectangle){export_window_pos.x + 5, (export_window_pos.y + 16 * 8) + 5, 500, 16})) {
    size = 1024;
    export_mode = 3;
  } else if (export_mode == 3) {
    int scale = size / GRID_WIDTH;
    Image img = GenImageColor(size, size, BLANK);

    for (int idx = 0; idx < TOTAL_PIXELS; idx++) {
      int col = idx % GRID_WIDTH;
      int row = idx / GRID_WIDTH;
      Color clr = pixels[idx] ? BLACK : WHITE;

      for (int py = 0; py < scale; py++) {
        for (int px = 0; px < scale; px++) {
          int x = col * scale + px;
          int y = row * scale + py;
          ImageDrawPixel(&img, x, y, clr);
        }
      }
    }

    ExportImage(img, "meowpixel_export_image.png");
    UnloadImage(img);
    export_mode = 0;
  }

  return 0;
}


void draw(void) {
  BeginDrawing();
    if (export_mode == 0) {
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
    } else if (export_mode == 1) {
      ClearBackground(WHITE);

      for (int i = 0; i < TOTAL_PIXELS; i++) {
        int col = i % GRID_WIDTH;
        int row = i / GRID_WIDTH;

        int posX = col * pixel_size_full;
        int posY = row * pixel_size_full;

        DrawRectangle(posX, posY, pixel_size_full, pixel_size_full, pixels[i] ? BLACK : WHITE);
      }
    } else if (export_mode == 2) {
      ClearBackground(BLACK);

      DrawRectangle(export_window_pos.x - 1, export_window_pos.y - 1, 502, 252, WHITE);
      DrawRectangle(export_window_pos.x, export_window_pos.y, 500, 250, DARKGRAY);

      DrawText("Choose size:", export_window_pos.x + 5, export_window_pos.y + 5, 16, WHITE);
      DrawText("16x16", export_window_pos.x + 5, (export_window_pos.y + 16 * 2) + 5, 16, WHITE);
      DrawText("32x32", export_window_pos.x + 5, (export_window_pos.y + 16 * 3) + 5, 16, WHITE);
      DrawText("64x64", export_window_pos.x + 5, (export_window_pos.y + 16 * 4) + 5, 16, WHITE);
      DrawText("128x128", export_window_pos.x + 5, (export_window_pos.y + 16 * 5) + 5, 16, WHITE);
      DrawText("256x256", export_window_pos.x + 5, (export_window_pos.y + 16 * 6) + 5, 16, WHITE);
      DrawText("512x512", export_window_pos.x + 5, (export_window_pos.y + 16 * 7) + 5, 16, WHITE);
      DrawText("1024x1024", export_window_pos.x + 5, (export_window_pos.y + 16 * 8) + 5, 16, WHITE);
    }

    DrawRectangle(0, 0, GetScreenWidth(), 20, WHITE);
    DrawText("meowpixel v0.4 beta", 5, 2, 16, BLACK);
    DrawText("Export Mode", 225, 2, 16, export_mode ? SKYBLUE : BLACK);
    DrawText("|", 336, 2, 16, BLACK);
    DrawText("Clear Screen", 350, 2, 16, BLACK);
    DrawText("Exit", GetScreenWidth() - MeasureText("Exit", 16) - 5, 2, 16, RED);
    DrawLine(0, 20, GetScreenWidth(), 20, BLACK);
  EndDrawing();
}
