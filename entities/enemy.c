#include "raylib.h"
#include "game.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int size_enemies[3] = {100, 100, 110};
char shortwords[200][15]; 
char mediumwords[100][15];
char longwords[100][15];

float wave_timer = 0.0f;
float wave_interval = 10.0f;
float wave_clear_timer = 2.0f;
int enemies_per_wave = 3;
int enemies_spawned_this_wave = 0;
int current_wave = 1;
bool wave_clear = false;


void InitTexts(void) {
    FILE *fileshort = fopen("txt_files/shortwords.txt", "r");
    FILE *filemedium = fopen("txt_files/mediumwords.txt", "r");
    FILE *filelong = fopen("txt_files/longwords.txt", "r");

    for (int i = 0; i < 200; i++) {
        fscanf(fileshort, "%s\n", shortwords[i]);
        if (i < 100) {
            fscanf(filemedium, "%s\n", mediumwords[i]);
            fscanf(filelong, "%s\n", longwords[i]);
        }
    }
    fclose(fileshort);
    fclose(filemedium);
    fclose(filelong);
}

void UnloadEnemyTextures() {
    UnloadTexture(enemy_textures[0]);
    UnloadTexture(enemy_textures[1]);
    UnloadTexture(enemy_textures[2]);
}

void DrawEnemies(EnemyList* enemy_list, Font myfont, Texture2D enemyTextures[]) {
    for (int i = 0; i < enemy_list->qty_enemies; i++) {
        Enemy enemy = enemy_list->enemies[i];
        Texture2D texture = enemyTextures[enemy.texture_index];

        Vector2 center = {
            enemy.rect.x + enemy.rect.width / 2.0f,
            enemy.rect.y + enemy.rect.height / 2.0f
        };
        Vector2 origin = { texture.width / 2.0f, texture.height / 2.0f };

        // Desenha o inimigo com textura e rotação
        DrawTexturePro(texture,
            (Rectangle){ 0, 0, (float)texture.width, (float)texture.height },
            (Rectangle){ center.x, center.y, enemy.rect.width, enemy.rect.height },
            origin,
            enemy.angle,
            WHITE
        );

        // Medida do texto
        int word_size = strlen(enemy.word);
        float text_size = 30.0f;
        float spacing = 0;
        Vector2 word_size_measure = MeasureTextEx(myfont, enemy.word, text_size, spacing);
        float text_x = center.x - word_size_measure.x / 2;
        float text_y = center.y + enemy.rect.height / 2.0f;

        // Desenha as letras da palavra
        if (enemy.index_typing == 0) {
            DrawTextEx(myfont, enemy.word, (Vector2){ text_x, text_y }, text_size, spacing, WHITE);
        } else {
            for (int j = 0; j < word_size; j++) {
                if (j < enemy.index_typing) {
                    // Letra já digitada, não desenha
                    continue;
                } else {
                    Color letterColor = enemy.locked ? BLUE : WHITE;
                    DrawTextEx(myfont, TextFormat("%c", enemy.word[j]),
                               (Vector2){ text_x + (15 * j), text_y }, text_size, spacing, letterColor);
                }
            }
        }
    }

    if (wave_clear == true) {
        if (wave_clear_timer >= 0) { 
            DrawText(TextFormat("WAVE %d COMPLETA!", current_wave - 1), SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 - 30, 30, YELLOW);
            DrawText(TextFormat("Próxima em %.1f", wave_clear_timer), SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 10, 20, YELLOW);
            wave_clear_timer -= GetFrameTime();
        } else {
            wave_clear = false;
            wave_clear_timer = 3.0f;
            enemies_spawned_this_wave = 0;
        }
    }

}



void SpawnEnemy(EnemyList* enemy_list) {
    int i;
    float chance = GetRandomValue(0, 100);

    if (chance >= 90) {
        i = 2;
    } else if (50 <= chance && chance < 90) {
        i = 1;
    } else {
        i = 0;
    }

    int size_chosen = size_enemies[i];

    Enemy enemy;
    enemy.color = RED;
    enemy.rect = (Rectangle) {GetRandomValue(0, SCREEN_WIDTH - size_chosen), -100, size_chosen, size_chosen};
    
    if (i == 0) enemy.speed = 2.0f;
    else if (i == 1) enemy.speed = 1.5f;
    else if (i == 2) enemy.speed = 1.0f;

    enemy.locked = 0;
    enemy.index_typing = 0;
    enemy.delay_speed = 0;

    if (i == 0) {
        strcpy(enemy.word, shortwords[GetRandomValue(0,199)]);
    } else if (i == 1) {
        strcpy(enemy.word, mediumwords[GetRandomValue(0,99)]);
    } else {
        strcpy(enemy.word, longwords[GetRandomValue(0,99)]);
    }

    int same_initial = 1;
    while (same_initial) {
        same_initial = 0;
        for (int i = 0; i < enemy_list->qty_enemies; i++) {
            if (enemy_list->enemies[i].word[0] == enemy.word[0]) {
                same_initial = 1;
                if (i == 0) {
                    strcpy(enemy.word, shortwords[GetRandomValue(0,199)]);
                } else if (i == 1) {
                    strcpy(enemy.word, mediumwords[GetRandomValue(0,99)]);
                } else if (i == 2) {
                    strcpy(enemy.word, longwords[GetRandomValue(0,99)]);
                }
                break;
            }
        }
    }
    
    enemy.texture_index = i; // 0, 1 ou 2, baseado no tamanho
    enemy.angle = 0.0f;
    enemy.health = strlen(enemy.word);
    enemy_list->enemies[enemy_list->qty_enemies] = enemy;
    enemy_list->qty_enemies++;
}

void UpdateEnemyWaves(EnemyList* enemy_list) {
    if (wave_clear == false) {
        if (enemies_spawned_this_wave < enemies_per_wave) {
            wave_timer += GetFrameTime();
            if (wave_timer > wave_interval / enemies_per_wave) {
                wave_timer = 0;
                SpawnEnemy(enemy_list);
                enemies_spawned_this_wave++;
            }
        }
    }
    if (enemies_spawned_this_wave == enemies_per_wave && enemy_list->qty_enemies == 0) {
        wave_clear = true;
        enemies_per_wave += 2;
        current_wave++;
    }
}

void RemoveEnemy(EnemyList* enemy_list, int index_enemy, Score* score) {
    IncreaseScore(score);
    PlaySound(morteSound); // Toca o som de morte quando inimigo morre
    for (int i = index_enemy; i < enemy_list->qty_enemies; i++) {
        enemy_list->enemies[i] = enemy_list->enemies[i + 1];
    }
    enemy_list->qty_enemies--;
} 

void MoveEnemies(EnemyList* enemy_list, Player* player, int* freeze, double* time_pass, Power_up_list* power_up_list, Score* score) {

    if (*freeze){
        wave_timer -= GetFrameTime();
        Power_up_time(enemy_list, *time_pass, 2.0, freeze); // 2.0 subtstituir pela velocidade padrão do jogo }
    }

    for (int i = 0; i < enemy_list->qty_enemies; i++) {
        Enemy enemy = enemy_list->enemies[i];
        Vector2 enemy_center = {enemy.rect.x + enemy.rect.width / 2, enemy.rect.y + enemy.rect.height / 2};
        float dx = player->rect.x - enemy_center.x;
        float dy = player->rect.y - enemy_center.y;
        float distance = sqrt(dx*dx + dy*dy);

        if (enemy.health == 0) {
            RemoveEnemy(enemy_list, i, score);
            i--;
    
            int chance = rand() % 10 + 1;
            if (chance <= 2){ // power up de congelar o tempo
                Adicionar_power_up(1, power_up_list);
            }

            else if (chance <= 4){ // power up de destruir alguns dos asteroides
                Adicionar_power_up(2,power_up_list);
            }
            continue;
        }

        if (!CheckCollisionRecs(player->rect, enemy.rect)) {
            enemy_list->enemies[i].rect.x += (dx / distance) * enemy.speed;
            enemy_list->enemies[i].rect.y += (dy / distance) * enemy.speed;
        } else {
            if (enemy_list->enemies[i].locked) {
                state = NOTLOCKED;
            }
            LoseLife(player);
            RemoveEnemy(enemy_list, i, score);
            i--;
        }
    }
}

void DelayEnemies(EnemyList* enemy_list) {
    for (int i = 0; i < enemy_list->qty_enemies; i++) {
        Enemy enemy = enemy_list->enemies[i];
        if (enemy.delay_speed > 0) {
            enemy_list->enemies[i].speed = 0.5;
            enemy_list->enemies[i].delay_speed -= GetFrameTime();
        } else {
            enemy_list->enemies[i].delay_speed = 0;
            if (enemy_list->enemies[i].texture_index == 0) enemy_list->enemies[i].speed = 2.0f;
            else if (enemy_list->enemies[i].texture_index == 1) enemy_list->enemies[i].speed = 1.5f;
            else if (enemy_list->enemies[i].texture_index == 2) enemy_list->enemies[i].speed = 1.0f;
        }
    }
}

