// lives.c - Sistema de vidas e Game Over para Typing Attack
#include "raylib.h"
#include "game.h"

Texture2D heart;

// Inicializa o sistema de vidas para o player
void InitLives(Player* player) {
    heart = LoadTexture("sprites/heart.png");
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

// Reinicia o jogo após Game Over
void ResetGame(Player* player, EnemyList* enemy_list, ProjectileList* projectile_list) {
    // Reinicia o jogador
    CreatePlayer(player);
    
    // Limpa os inimigos
    for (int i = 0; i < 20; i++) {
        enemy_list->enemies[i].health = -1;
    }
    enemy_list->qty_enemies = 0;
    
    // Limpa os projéteis
    for (int i = 0; i < 50; i++) {
        projectile_list->projectiles[i].speed = 0;
    }
    projectile_list->qty_projectiles = 0;
    
    // Reinicia o estado de digitação
    state = NOTLOCKED;
    current_wave = 1;
    wave_clear = false;
    score.points = 0;
    enemies_spawned_this_wave = 0;
}