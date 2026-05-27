#include "raylib.h"


void setup(void);
void unsetup(void);

void functional(void);
void draw(void);

int pixels[100];


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
}

void unsetup(void) {
  CloseWindow();
}


void functional(void) {

}

void draw(void) {
  BeginDrawing();
    ClearBackground((Color){255, 255, 245, 255});

    for (int i = 0; i < 100; i++) {
      int col = i % 10;
      int row = i / 10;

      int posX = 125 + (col * 100);
      int posY = 125 + (row * 100);

      DrawRectangle(posX, posY, 100, 100, pixels[i] ? BLACK : WHITE);
    }

    for (int i = 125; i <= 1125; i += 100)
        DrawLine(i, 125, i, 1125, BLACK);
    for (int i = 125; i <= 1125; i += 100)
        DrawLine(125, i, 1125, i, BLACK);
  EndDrawing();
}
