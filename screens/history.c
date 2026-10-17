#include "raylib.h"
#include "game.h"

char text[500] = "Os invasores galácticos, em sua grande \"sabedoria\", decidiram usar palavras aleatórias como escudo para suas naves. Mas o astuto guerreiro do CIn, o mais rápido digitador da galáxia, está pronto para detê-los! Com dedos mais rápidos que lasers e uma destreza inumana no teclado, ele defende a Terra da ameaça mais absurda do universo: um combate de digitação intergaláctico!";
int pos_x = 1280;
float scroll_speed = 350.0f;

void DrawHistory(Texture2D background, Font myfont) {
    ClearBackground(BLACK);
    DrawTexture(background, 0, 0, WHITE);
    DrawRectangle(0, (720/2 - 50), 1280, 100, WHITE);
    DrawTextEx(GetFontDefault(), text, (Vector2) {pos_x, (720/2 - 10)}, 30, 5, BLACK);

    DrawTextEx(GetFontDefault(), "Aperte ENTER para pular...", (Vector2) {50, (720/2 + 70)}, 30, 5, WHITE);
}

void UpdateHistory() {
    pos_x -= (int)(GetFrameTime() * scroll_speed);
    if (IsKeyPressed(KEY_ENTER)) {
        currentScreen = GAMEPLAY;
    }
}