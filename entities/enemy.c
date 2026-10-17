#include "game.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

static char shortwords[200][15];
static char mediumwords[100][15];
static char longwords[100][15];
static int short_count, medium_count, long_count;
static unsigned int next_enemy_id = 1;
static float wave_timer = 0.0f;
static const float wave_interval = 10.0f;
static float wave_clear_timer = 2.0f;
static int enemies_per_wave = 3;
int enemies_spawned_this_wave = 0;
int current_wave = 1;
bool wave_clear = false;

static int ReadWords(const char *path, char words[][15], int limit) {
    FILE *file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "Nao foi possivel abrir %s\n", path);
        return 0;
    }
    int count = 0;
    while (count < limit && fscanf(file, "%14s", words[count]) == 1) count++;
    fclose(file);
    return count;
}

void InitTexts(void) {
    short_count = ReadWords("txt_files/shortwords.txt", shortwords, 200);
    medium_count = ReadWords("txt_files/mediumwords.txt", mediumwords, 100);
    long_count = ReadWords("txt_files/longwords.txt", longwords, 100);
    if (!short_count) { strcpy(shortwords[0], "code"); short_count = 1; }
    if (!medium_count) { strcpy(mediumwords[0], "typing"); medium_count = 1; }
    if (!long_count) { strcpy(longwords[0], "keyboard"); long_count = 1; }
}

void ResetEnemyWaves(void) {
    wave_timer = 0.0f;
    wave_clear_timer = 2.0f;
    enemies_per_wave = 3;
    enemies_spawned_this_wave = 0;
    current_wave = 1;
    wave_clear = false;
    next_enemy_id = 1;
}

void UnloadEnemyTextures(void) {
    for (int i = 0; i < 3; i++) UnloadTexture(enemy_textures[i]);
}

void DrawEnemies(EnemyList* list, Font font, Texture2D textures[]) {
    for (int i = 0; i < list->qty_enemies; i++) {
        const Enemy *enemy = &list->enemies[i];
        Texture2D texture = textures[enemy->texture_index];
        Vector2 center = {enemy->rect.x + enemy->rect.width / 2,
                          enemy->rect.y + enemy->rect.height / 2};
        Vector2 origin = {texture.width / 2.0f, texture.height / 2.0f};
        DrawTexturePro(texture, (Rectangle){0, 0, (float)texture.width, (float)texture.height},
                       (Rectangle){center.x, center.y, enemy->rect.width, enemy->rect.height},
                       origin, enemy->angle, WHITE);
        float text_size = 30.0f;
        Vector2 measure = MeasureTextEx(font, enemy->word, text_size, 0.0f);
        float text_x = center.x - measure.x / 2;
        float text_y = center.y + enemy->rect.height / 2.0f;
        int len = (int)strlen(enemy->word);
        if (enemy->index_typing == 0) {
            DrawTextEx(font, enemy->word, (Vector2){text_x, text_y}, text_size, 0.0f, WHITE);
        } else {
            for (int j = enemy->index_typing; j < len; j++) {
                DrawTextEx(font, TextFormat("%c", enemy->word[j]),
                           (Vector2){text_x + 15 * j, text_y}, text_size, 0, enemy->locked ? BLUE : WHITE);
            }
        }
    }
    if (wave_clear) {
        DrawText(TextFormat("WAVE %d COMPLETA!", current_wave - 1),
                 SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 - 30, 30, YELLOW);
        DrawText(TextFormat("Proxima em %.1f", wave_clear_timer),
                 SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 10, 20, YELLOW);
    }
}

static bool SameInitial(const EnemyList *list, char letter) {
    for (int i = 0; i < list->qty_enemies; i++)
        if (list->enemies[i].word[0] == letter) return true;
    return false;
}

void SpawnEnemy(EnemyList* list) {
    const int capacity = (int)(sizeof(list->enemies) / sizeof(list->enemies[0]));
    if (list->qty_enemies >= capacity) return;

    int chance = GetRandomValue(0, 99);
    int type = chance >= 90 ? 2 : (chance >= 50 ? 1 : 0);
    char (*words)[15] = type == 0 ? shortwords : (type == 1 ? mediumwords : longwords);
    int count = type == 0 ? short_count : (type == 1 ? medium_count : long_count);

    int chosen = GetRandomValue(0, count - 1);
    // Bounded search: all initials may already be occupied.
    for (int tries = 0; tries < count && SameInitial(list, words[chosen][0]); tries++)
        chosen = (chosen + 1) % count;
    // Allow a repeated initial rather than freezing the main loop.
    Enemy enemy = {0};
    int size = type == 2 ? 110 : 100;
    enemy.rect = (Rectangle){GetRandomValue(0, SCREEN_WIDTH - size), -100, size, size};
    enemy.color = RED;
    enemy.speed = type == 0 ? 2.0f : (type == 1 ? 1.5f : 1.0f);
    enemy.texture_index = type;
    strcpy(enemy.word, words[chosen]);
    enemy.health = (int)strlen(enemy.word);
    enemy.id = next_enemy_id++;
    list->enemies[list->qty_enemies++] = enemy;
}

void UpdateEnemyWaves(EnemyList* list) {
    if (wave_clear) {
        wave_clear_timer -= GetFrameTime();
        if (wave_clear_timer <= 0.0f) {
            wave_clear = false;
            wave_clear_timer = 2.0f;
            enemies_spawned_this_wave = 0;
            wave_timer = 0.0f;
        }
        return;
    }
    if (enemies_spawned_this_wave < enemies_per_wave) {
        wave_timer += GetFrameTime();
        if (wave_timer >= wave_interval / enemies_per_wave &&
            list->qty_enemies < (int)(sizeof(list->enemies)/sizeof(list->enemies[0]))) {
            wave_timer = 0.0f;
            SpawnEnemy(list);
            enemies_spawned_this_wave++;
        }
    }
    if (enemies_spawned_this_wave >= enemies_per_wave && list->qty_enemies == 0) {
        wave_clear = true;
        if (enemies_per_wave < 45) enemies_per_wave += 2;
        current_wave++;
    }
}

void RemoveEnemy(EnemyList* list, int index, Score* score, bool award_points) {
    if (index < 0 || index >= list->qty_enemies) return;
    if (award_points) {
        IncreaseScore(score);
        PlaySound(morteSound);
    }
    for (int i = index; i + 1 < list->qty_enemies; i++)
        list->enemies[i] = list->enemies[i + 1];
    list->qty_enemies--;
}

void MoveEnemies(EnemyList* list, Player* player, int* freeze, double* time_pass,
                 Power_up_list* powers, Score* score) {
    if (*freeze) Power_up_time(list, *time_pass, 0, freeze);
    float step = GetFrameTime() * 60.0f;
    for (int i = 0; i < list->qty_enemies; i++) {
        Enemy *enemy = &list->enemies[i];
        if (enemy->health <= 0) {
            RemoveEnemy(list, i, score, true);
            i--;
            int chance = GetRandomValue(1, 10);
            if (chance <= 2) Adicionar_power_up(1, powers);
            else if (chance <= 4) Adicionar_power_up(2, powers);
            continue;
        }
        Vector2 center = {enemy->rect.x + enemy->rect.width/2, enemy->rect.y + enemy->rect.height/2};
        float dx = player->rect.x - center.x;
        float dy = player->rect.y - center.y;
        float distance = sqrtf(dx * dx + dy * dy);
        if (CheckCollisionRecs(player->rect, enemy->rect)) {
            if (enemy->locked) state = NOTLOCKED;
            LoseLife(player);
            RemoveEnemy(list, i, score, false);
            i--;
            if (currentScreen == GAME_OVER) return;
        } else if (distance > 0.0001f) {
            enemy->rect.x += dx / distance * enemy->speed * step;
            enemy->rect.y += dy / distance * enemy->speed * step;
        }
    }
}

void DelayEnemies(EnemyList* list) {
    for (int i = 0; i < list->qty_enemies; i++) {
        Enemy *enemy = &list->enemies[i];
        if (enemy->delay_speed > 0) {
            enemy->delay_speed -= GetFrameTime();
            enemy->speed = 0.5f;
        } else {
            enemy->delay_speed = 0;
            enemy->speed = enemy->texture_index == 0 ? 2.0f : (enemy->texture_index == 1 ? 1.5f : 1.0f);
        }
    }
}
