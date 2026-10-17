#include "game.h"
#include <stdio.h>
#include <string.h>

static Button config, sound, music;
static Line sound_line, music_line;
static Rectangle sound_rec, music_rec;
static int soundPressed = 0, musicPressed = 0;
static int sound_vol = 75, music_vol = 75;

static int ClampPercent(int volume) {
    return volume < 0 ? 0 : (volume > 100 ? 100 : volume);
}

static void ApplyVolumes(void) {
    float effects = sound_vol / 100.0f;
    float background_music = music_vol / 100.0f;
    SetSoundVolume(botaoSound, effects);
    SetSoundVolume(morteSound, effects);
    SetSoundVolume(tiroSound, effects);
    SetSoundVolume(freezeSound, effects);
    SetSoundVolume(menuJogoSound, background_music);
}

static void UpdateSliders(void) {
    sound_rec = (Rectangle){sound_line.x - 5 + sound_vol * 3, sound_line.y - 10, 10, 20};
    music_rec = (Rectangle){music_line.x - 5 + music_vol * 3, music_line.y - 10, 10, 20};
}

void SetSettings(void) {
    FILE *file = fopen("txt_files/settings.txt", "r");
    if (file) {
        int sound_read, music_read;
        if (fscanf(file, "%d %d", &sound_read, &music_read) == 2) {
            sound_vol = ClampPercent(sound_read);
            music_vol = ClampPercent(music_read);
        }
        fclose(file);
    }
    config = (Button){{rec.x - 30, rec.y - 240, rec.width, rec.height},
                       "Configuracoes", BLACK, RAYWHITE};
    sound = (Button){{rec.x, rec.y - 90, rec.width, rec.height}, "Som", BLACK, RAYWHITE};
    music = (Button){{rec.x, rec.y + 60, rec.width, rec.height}, "Musica", BLACK, RAYWHITE};
    sound_line = (Line){SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 - 40, 300, RAYWHITE};
    music_line = (Line){SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 110, 300, RAYWHITE};
    UpdateSliders();
    ApplyVolumes();
}

static void SaveSettings(void) {
    FILE *file = fopen("txt_files/settings.txt", "w");
    if (!file) {
        fprintf(stderr, "Nao foi possivel salvar configuracoes.\n");
        return;
    }
    fprintf(file, "%d\n%d\n", sound_vol, music_vol);
    fclose(file);
}

void DrawSettings(void) {
    ClearBackground(BLACK);
    DrawTexturePro(background,
                   (Rectangle){0, 0, background.width, background.height},
                   (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, (Vector2){0, 0}, 0, WHITE);
    DrawText(config.text, 280, 130, 80, RAYWHITE);
    DrawText(sound.text, SCREEN_WIDTH/2 - 55, sound.rec.y, font_size, RAYWHITE);
    DrawText(music.text, SCREEN_WIDTH/2 - 75, music.rec.y, font_size, RAYWHITE);
    DrawLine((int)sound_line.x, (int)sound_line.y, (int)(sound_line.x + sound_line.size), (int)sound_line.y, RAYWHITE);
    DrawLine((int)music_line.x, (int)music_line.y, (int)(music_line.x + music_line.size), (int)music_line.y, RAYWHITE);
    DrawRectangleRec(sound_rec, WHITE);
    DrawRectangleRec(music_rec, WHITE);
    DrawText(TextFormat("%d%%", sound_vol), (int)(sound_line.x + sound_line.size + 20), (int)sound_line.y - 10, 20, WHITE);
    DrawText(TextFormat("%d%%", music_vol), (int)(music_line.x + music_line.size + 20), (int)music_line.y - 10, 20, WHITE);
    DrawText(back.text, (int)back.rec.x, (int)back.rec.y, font_size,
             CheckCollisionPointRec(GetMousePosition(), back.rec) ? GRAY : RAYWHITE);
}

void UpdateSettings(void) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mouse = GetMousePosition();
        if (CheckCollisionPointRec(mouse, (Rectangle){sound_line.x - 10, sound_line.y - 15, 320, 30})) soundPressed = 1;
        else if (CheckCollisionPointRec(mouse, (Rectangle){music_line.x - 10, music_line.y - 15, 320, 30})) musicPressed = 1;
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (soundPressed) sound_vol = ClampPercent((int)((GetMouseX() - sound_line.x)/3.0f + 0.5f));
        if (musicPressed) music_vol = ClampPercent((int)((GetMouseX() - music_line.x)/3.0f + 0.5f));
        if (soundPressed || musicPressed) {
            UpdateSliders();
            ApplyVolumes();
        }
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        if (soundPressed || musicPressed) SaveSettings();
        soundPressed = musicPressed = 0;
        if (CheckCollisionPointRec(GetMousePosition(), back.rec)) {
            PlaySound(botaoSound);
            currentScreen = paused ? PAUSE : MENU;
        }
    }
}
