#include "raylib.h"
#include "game.h"
#include <math.h>

float angle_enemy = 0.0f;
Texture2D nave;
Texture2D mira;

void CreatePlayer(Player* player) {
    float scale = 2.0f; // aumenta o tamanho da sprite (1.5x)
    float spriteSize = 200.0f * scale;

    player->rect = (Rectangle) {
        1280 / 2,              // posição x centralizada
        650,                   // posição y mais alta que antes (era 600)
        spriteSize,            // largura
        spriteSize             // altura
    };
    
    player->angle = 0;
    player->color = YELLOW;
    nave = LoadTexture("sprites/nave.png");
    mira = LoadTexture("sprites/mira.png");

    InitLives(player);  // Inicializa o sistema de vidas
}

void UnloadPlayer() {
    UnloadTexture(nave);
    UnloadTexture(mira);
}

void DrawPlayer(Player* player) {
    Vector2 center = {player->rect.width / 2, player->rect.height / 2};

    if (IsPlayerInvincible(player)) {
        // Faz o jogador piscar quando está invencível
        if ((int)(player->invincible * 10) % 2 == 0) {
            return;  // Não desenha o jogador neste frame (efeito de piscar)
        }
    }

    if (abs(angle_enemy - player->angle) > 6) {
        if (angle_enemy < player->angle) {
            player->angle -= 6.0f;
        } else if (angle_enemy > player->angle) {
            player->angle += 6.0f;
        }
    } else {
        player->angle = angle_enemy;
    }

    // desenha a nave estática atrás
    DrawTexturePro(nave, (Rectangle){0, 0, nave.width, nave.height},
                         player->rect, center, 0.0f, WHITE);

    // desenha a mira rotacionada por cima
    DrawTexturePro(mira, (Rectangle){0, 0, mira.width, mira.height},
                         player->rect, center, player->angle, WHITE);
}

void TurnToEnemy(Player* player, EnemyList* enemy_list) {
    for (int i = 0; i < enemy_list->qty_enemies; i++) {
        Enemy enemy = enemy_list->enemies[i];
        Vector2 enemy_center = {enemy.rect.x + enemy.rect.width / 2, enemy.rect.y + enemy.rect.height / 2};
        Vector2 player_center = {player->rect.x, player->rect.y};

        if (enemy.locked) {
            float dx = player_center.x - enemy_center.x;
            float dy = player_center.y - enemy_center.y;
            int angle = atan2f(dx, dy) * (-180 / PI); // use atan2f pra mais precisão
            angle_enemy = angle;
        }
    }
}
