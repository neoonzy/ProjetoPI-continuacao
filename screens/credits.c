#include "raylib.h"
#include "game.h"

Button credit, names;

void SetCredits(void) {
    credit = (Button) {{rec.x - 30, rec.y - 240, rec.width, rec.height}, "Créditos", BLACK, RAYWHITE};
    names = (Button) {{rec.x - 300, rec.y - 20, 400, 300}, "Alisson Vinicius\nDiego Juan\nIan Monteiro\nLuis Eduardo Lustosa\nMiguel Câmara\nPedro Inácio\nThiago Tenório", BLACK, RAYWHITE};
}

void DrawCredits(void) {
    SetCredits();
    Texture2D background = LoadTexture("sprites/background.png"); // AJEITAR DPS
    ClearBackground(BLACK);    
    DrawTexturePro(background,
    (Rectangle){ 0, 0, background.width, background.height },
    (Rectangle){ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT },
    (Vector2){ 0, 0 },
    0.0f,
    WHITE);

    DrawText(credit.text, (SCREEN_WIDTH - 77*5)/2, credit.rec.y + (credit.rec.height - 100)/2, 100, credit.text_color);
    DrawText(names.text, names.rec.x + 200, name.rec.y + 240, 40, names.text_color);
    DrawText(back.text,   (SCREEN_WIDTH - 33*5)/2, back.rec.y  + (back.rec.height - font_size)/2, font_size, back.text_color);

    if (CheckCollisionPointRec(GetMousePosition(), back.rec)) {
        DrawText(back.text, (SCREEN_WIDTH - 33*5)/2, back.rec.y  + (back.rec.height - font_size)/2, font_size, GRAY);
    }
}

void UpdateCredits(void) {
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), back.rec)) {
        PlaySound(botaoSound); // Toca o som de botão
        currentScreen = MENU;
    }
}
