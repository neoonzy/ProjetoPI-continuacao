#include "raylib.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Button config, sound, music, back;
Line sound_line, music_line, line = {(SCREEN_WIDTH)/2, (SCREEN_HEIGHT)/2, 300, RAYWHITE};
Rectangle sound_rec, music_rec;
int soundPressed = 0;
int musicPressed = 0;
FILE *sett_file;
int sound_vol, music_vol;

void SetSettings(void) {
    sett_file = fopen("txt_files/settings.txt", "r");
    fscanf(sett_file, "%d", &sound_vol);
    fscanf(sett_file, "%d", &music_vol);
    fclose(sett_file);

    config.rec = (Rectangle) {rec.x - 30, rec.y - 240, rec.width, rec.height};
    sound.rec  = (Rectangle) {rec.x, rec.y -  90, rec.width, rec.height};
    music.rec  = (Rectangle) {rec.x, rec.y +  60, rec.width, rec.height};

    strcpy(config.text, "Configurações");  config.text_color = RAYWHITE;
    strcpy(sound.text, "Som");             sound.text_color = RAYWHITE;
    strcpy(music.text, "Música");          music.text_color = RAYWHITE;

    sound_line = (Line) {line.x - line.size/2, line.y -  40, line.size, line.color};
    music_line = (Line) {line.x - line.size/2, line.y + 110, line.size, line.color};
    sound_rec = (Rectangle) {line.x - 5 + (sound_vol - 50)*3, line.y -  40 - 10, 10, 20};
    music_rec = (Rectangle) {line.x - 5 + (music_vol - 50)*3, line.y + 110 - 10, 10, 20};

    SetSoundVolume(botaoSound, sound_vol/10);
    SetSoundVolume(morteSound, sound_vol/10);
    SetSoundVolume(tiroSound, sound_vol/10);
    SetSoundVolume(menuJogoSound, music_vol/10);
}

void DrawSettings(void) {
    SetSettings();
    ClearBackground(BLACK);
    
    DrawTexturePro(background,
    (Rectangle){ 0, 0, background.width, background.height },
    (Rectangle){ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT },
    (Vector2){ 0, 0 },
    0.0f,
    WHITE);

    DrawText(config.text, (SCREEN_WIDTH - 146*5)/2, config.rec.y + (config.rec.height - 100)/2, 100, config.text_color);
    DrawText(sound.text,  (SCREEN_WIDTH - 18*5)/2, sound.rec.y + (sound.rec.height - font_size)/2, font_size, sound.text_color);
    DrawText(music.text,  (SCREEN_WIDTH - 33*5)/2, music.rec.y + (music.rec.height - font_size)/2, font_size, music.text_color);
    DrawText(back.text,   (SCREEN_WIDTH - 33*5)/2, back.rec.y  + (back.rec.height - font_size)/2, font_size, back.text_color);
    
    DrawLine(sound_line.x, sound_line.y, sound_line.x + sound_line.size, sound_line.y, sound_line.color);
    DrawLine(music_line.x, music_line.y, music_line.x + music_line.size, music_line.y, music_line.color);
    DrawRectangle(sound_rec.x, sound_rec.y, sound_rec.width, sound_rec.height, WHITE);
    DrawRectangle(music_rec.x, music_rec.y, music_rec.width, music_rec.height, WHITE);

    // back
    if (CheckCollisionPointRec(GetMousePosition(), back.rec)) {
        DrawText(back.text, (SCREEN_WIDTH - 33*5)/2, back.rec.y  + (back.rec.height - font_size)/2, font_size, GRAY);
    }
    // sound
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), sound_rec)) soundPressed = 1;
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) soundPressed = 0;

    if (soundPressed && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        sett_file = fopen("txt_files/settings.txt", "w");

        DrawRectangle(sound_rec.x, sound_rec.y, sound_rec.width, sound_rec.height, BLACK);
        DrawLine(sound_line.x, sound_line.y, sound_line.x + sound_line.size, sound_line.y, sound_line.color);

        if (GetMouseX() < sound_line.x) {
            fprintf(sett_file, "%d\n", 0);
            sound_rec.x = sound_line.x;
            DrawRectangle(sound_rec.x, sound_rec.y, sound_rec.width, sound_rec.height, WHITE);
        } else
        if (GetMouseX() > sound_line.x + sound_line.size) {
            fprintf(sett_file, "%d\n", 100);
            sound_rec.x = sound_line.x + sound_line.size;
            DrawRectangle(sound_rec.x, sound_rec.y, sound_rec.width, sound_rec.height, WHITE);
        } else {
            fprintf(sett_file, "%d\n", (int) (GetMouseX() - sound_line.x)/3);
            sound_rec.x = GetMouseX() - sound_rec.width/2;
            DrawRectangle(sound_rec.x, sound_rec.y, sound_rec.width, sound_rec.height, WHITE);
        }
        fprintf(sett_file, "%d\n", music_vol);
        fclose(sett_file);
    }
    // music
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), music_rec)) musicPressed = 1;
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) musicPressed = 0;

    if (musicPressed && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        sett_file = fopen("txt_files/settings.txt", "w");
        fprintf(sett_file, "%d\n", sound_vol);

        DrawRectangle(music_rec.x, music_rec.y, music_rec.width, music_rec.height, BLACK);
        DrawLine(music_line.x, music_line.y, music_line.x + music_line.size, music_line.y, music_line.color);

        if (GetMouseX() < music_line.x) {
            fprintf(sett_file, "%d\n", 0);
            music_rec.x = music_line.x;
            DrawRectangle(music_rec.x, music_rec.y, music_rec.width, music_rec.height, WHITE);
        } else
        if (GetMouseX() > music_line.x + music_line.size) {
            fprintf(sett_file, "%d\n", 100);
            music_rec.x = music_line.x + music_line.size;
            DrawRectangle(music_rec.x, music_rec.y, music_rec.width, music_rec.height, WHITE);
        } else {
            fprintf(sett_file, "%d\n", (int) (GetMouseX() - music_line.x)/3);
            music_rec.x = GetMouseX() - music_rec.width/2;
            DrawRectangle(music_rec.x, music_rec.y, music_rec.width, music_rec.height, WHITE);
        }
        fclose(sett_file);
    }
}

void UpdateSettings(void) {
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), back.rec)) {
        PlaySound(botaoSound); // Toca o som de botão
        if (paused) {
            currentScreen = PAUSE;
        } else {
            currentScreen = MENU;
        }
    }
}