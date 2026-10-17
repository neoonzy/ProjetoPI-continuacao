#include "raylib.h"
#include "game.h"

Button game_over, restart, back_menu;

void SetGameOver(void) {
    game_over = (Button) {{rec.x - 30, rec.y - 240, rec.width, rec.height}, "Game Over", BLACK, RED};
    restart =   (Button) {{rec.x, rec.y + 60, rec.width, rec.height},   "Reiniciar", RAYWHITE, BLACK};
}

void DrawGameOver(void) {
    SetGameOver();
    Texture2D background = LoadTexture("sprites/background.png");
    ClearBackground(BLACK);    
    DrawTexturePro(
        background,
        (Rectangle){0, 0, background.width, background.height},
        (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT},
        (Vector2){ 0, 0 },
        0.0f,
        WHITE
    );

    DrawRectangleRounded(restart.rec, 0.5, 10, restart.rec_color);
    DrawRectangleRounded(back_menu.rec, 0.5, 10, back_menu.rec_color);

    DrawText(game_over.text, (SCREEN_WIDTH - 150*5)/2, game_over.rec.y + (game_over.rec.height - 100)/2, 150, game_over.text_color);
    DrawText(restart.text, (SCREEN_WIDTH - 35*5)/2, restart.rec.y + (restart.rec.height - font_size)/2, font_size, restart.text_color);
    DrawText(back_menu.text, (SCREEN_WIDTH - 25*5)/2, back_menu.rec.y + (back_menu.rec.height - font_size)/2, font_size, back_menu.text_color);

    if (CheckCollisionPointRec(GetMousePosition(), restart.rec)) {
        DrawRectangleRounded(restart.rec, 0.5, 10, restart.text_color);
        DrawText(restart.text, (SCREEN_WIDTH - 35*5)/2, restart.rec.y + (restart.rec.height - font_size)/2, font_size, restart.rec_color);
    }
    if (CheckCollisionPointRec(GetMousePosition(), back_menu.rec)) {
        DrawRectangleRounded(back_menu.rec, 0.5, 10, back_menu.text_color);
        DrawText(back_menu.text, (SCREEN_WIDTH - 25*5)/2, back_menu.rec.y + (back_menu.rec.height - font_size)/2, font_size, back_menu.rec_color);
    }
}

void UpdateGameOver(void) {
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), restart.rec)) {
        PlaySound(botaoSound); // Toca o som de botão
        currentScreen = GAMEPLAY;
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), back_menu.rec)) {
        PlaySound(botaoSound); // Toca o som de botão
        currentScreen = MENU;
    }
}
