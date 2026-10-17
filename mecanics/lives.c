// lives.c - Sistema de vidas e Game Over para Typing Attack
#include "raylib.h"
#include "game.h"


// Inicializa o sistema de vidas para o player
void InitLives(Player* player) {
    player->lives = 3;       // Inicializa com 3 vidas
    player->invincible = 0;  // Não está invencível no início
}

// Reduz uma vida do jogador e ativa invencibilidade temporária
void LoseLife(Player* player) {
    // Verificando se o jogador não está invencível
    if (player->invincible <= 0) {
        player->lives--;  // Reduz uma vida
        player->invincible = 2.0f;  // Dá 2 segundos de invencibilidade
        
        // Verificando se o jogador perdeu todas as vidas
        if (player->lives <= 0) {
            currentScreen = GAME_OVER;
        }
    }
}

// Atualiza o estado de invencibilidade do jogador
void UpdateInvincibility(Player* player) {
    if (player->invincible > 0) {
        player->invincible -= GetFrameTime();
    }
}

// Verifica se o jogador está invencível
bool IsPlayerInvincible(Player* player) {
    return player->invincible > 0;
}

// Desenha as vidas na interface
void DrawLives(Player* player) {
    // Desenha círculos representando as vidas
    for (int i = 0; i < player->lives; i++) {
        DrawTexturePro(heart, (Rectangle) {0, 0, 1200, 1125}, (Rectangle) {30 + (i * 30), 30, 30, 28}, (Vector2) {0,0}, 0, WHITE);
        // DrawCircle(30 + i * 30, 30, 10, RED);
    }
    
    // Desenha texto com número de vidas
    DrawText(TextFormat("VIDAS: %d", player->lives), SCREEN_WIDTH - 150, 20, 30, WHITE);
}

// Reset only on restart or return to menu.
void ResetGame(GameState* game) {
    CreatePlayer(&game->player);
    game->enemy_list.qty_enemies = 0;
    game->projectile_list.qty_projectiles = 0;
    game->power_up_list.number_power_ups = 0;
    for (int i = 0; i < game->power_up_list.max_power_up; i++) game->power_up_list.power_up[i].type = 0;
    game->freeze = 0;
    game->time_pass = 0.0;
    InitScore(&game->score);
    ResetEnemyWaves();
    state = NOTLOCKED;
    paused = 0;
}
