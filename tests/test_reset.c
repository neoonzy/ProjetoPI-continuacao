#include "game.h"
#include <assert.h>
#include <stdio.h>

// Minimal test doubles: no window or audio context required.
GameScreen currentScreen = GAME_OVER;
StateTyping state = LOCKONENEMY;
int current_wave = 9;
bool wave_clear = true;
int enemies_spawned_this_wave = 7;
int paused = 1;
Texture2D heart;

void CreatePlayer(Player *player) {
    player->lives = 3;
    player->invincible = 0.0f;
}
void InitScore(Score *score) { score->points = 0; }
void ResetEnemyWaves(void) {
    current_wave = 1;
    wave_clear = false;
    enemies_spawned_this_wave = 0;
}

int main(void) {
    GameState game = {0};
    game.player.lives = -1;
    game.enemy_list.qty_enemies = 5;
    game.projectile_list.qty_projectiles = 10;
    game.power_up_list.max_power_up = 3;
    game.power_up_list.number_power_ups = 3;
    for (int i = 0; i < 3; i++) game.power_up_list.power_up[i].type = i + 1;
    game.score.points = 120;
    game.freeze = 1;
    game.time_pass = 123.0;

    ResetGame(&game);
    assert(game.player.lives == 3);
    assert(game.player.invincible == 0.0f);
    assert(game.enemy_list.qty_enemies == 0);
    assert(game.projectile_list.qty_projectiles == 0);
    assert(game.power_up_list.number_power_ups == 0);
    for (int i = 0; i < 3; i++) assert(game.power_up_list.power_up[i].type == 0);
    assert(game.score.points == 0);
    assert(game.freeze == 0);
    assert(game.time_pass == 0.0);
    assert(current_wave == 1 && !wave_clear && enemies_spawned_this_wave == 0);
    assert(state == NOTLOCKED && paused == 0);
    puts("ResetGame: OK");
    return 0;
}
