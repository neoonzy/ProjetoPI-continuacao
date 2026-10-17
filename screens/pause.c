#include "raylib.h"
#include "game.h"

Button pause, resume, settings, back_menu;

void SetPause(void) {
    pause =     (Button) {{rec.x - 30, rec.y - 240, rec.width, rec.height}, "Pausado",  BLACK, RAYWHITE};
    resume =    (Button) {{rec.x, rec.y,       rec.width, rec.height}, "Continuar",     RAYWHITE, BLACK};
    settings =  (Button) {{rec.x, rec.y + 110, rec.width, rec.height}, "Configurações", RAYWHITE, BLACK};
}

void DrawPause(void) {
    SetPause();
    ClearBackground(BLACK);
    
    DrawTexturePro(
        background,
        (Rectangle){0, 0, background.width, background.height},
        (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT},
        (Vector2){ 0, 0 },
        0.0f,
        WHITE
    );

    DrawRectangleRounded(resume.rec, 0.5, 10, resume.rec_color);
    DrawRectangleRounded(settings.rec, 0.5, 10, settings.rec_color);
    DrawRectangleRounded(back_menu.rec, 0.5, 10, back_menu.rec_color);

    DrawText(pause.text, (SCREEN_WIDTH - 77*5)/2, pause.rec.y + (pause.rec.height - 100)/2, 100, pause.text_color);
    DrawText(resume.text, (SCREEN_WIDTH - 50*5)/2, resume.rec.y + (resume.rec.height - font_size)/2, font_size, resume.text_color);
    DrawText(settings.text, (SCREEN_WIDTH - 73*5)/2, settings.rec.y + (settings.rec.height - font_size)/2, font_size, settings.text_color);
    DrawText(back_menu.text, (SCREEN_WIDTH - 25*5)/2, back_menu.rec.y + (back_menu.rec.height - font_size)/2, font_size, back_menu.text_color);

    if (CheckCollisionPointRec(GetMousePosition(), resume.rec)) {
        DrawRectangleRounded(resume.rec, 0.5, 10, resume.text_color);
        DrawText(resume.text, (SCREEN_WIDTH - 50*5)/2, resume.rec.y + (resume.rec.height - font_size)/2, font_size, resume.rec_color);
    }
    if (CheckCollisionPointRec(GetMousePosition(), settings.rec)) {
        DrawRectangleRounded(settings.rec, 0.5, 10, settings.text_color);
        DrawText(settings.text, (SCREEN_WIDTH - 73*5)/2, settings.rec.y + (settings.rec.height - font_size)/2, font_size, settings.rec_color);
    }
    if (CheckCollisionPointRec(GetMousePosition(), back_menu.rec)) {
        DrawRectangleRounded(back_menu.rec, 0.5, 10, back_menu.text_color);
        DrawText(back_menu.text, (SCREEN_WIDTH - 25*5)/2, back_menu.rec.y + (back_menu.rec.height - font_size)/2, font_size, back_menu.rec_color);
    }
}

void UpdatePause(Player *player, EnemyList *enemy_list, ProjectileList *projectile_list) {
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), resume.rec)) {
        PlaySound(botaoSound); // Toca o som de botão
        currentScreen = GAMEPLAY;
        paused = 0;
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), settings.rec)) {
        PlaySound(botaoSound); // Toca o som de botão
        currentScreen = SETTINGS;
        paused = 1;
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), back_menu.rec)) {
        ResetGame(player, enemy_list, projectile_list);
        PlaySound(botaoSound); // Toca o som de botão
        currentScreen = MENU;
        paused = 0;
    }
}
