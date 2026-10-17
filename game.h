#ifndef GAME_H
#define GAME_H

#include "raylib.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

typedef struct Player {
    Rectangle rect;
    Color color;
    float angle;
    int lives;         // Número de vidas do jogador
    float invincible;  // Tempo de invencibilidade após perder uma vida
} Player;

typedef struct Enemy {
    Rectangle rect;
    Color color;
    float speed;
    char word[50];
    int health;
    int index_typing;
    int locked;
    float delay_speed;
    int texture_index;
    float angle;

} Enemy;

typedef struct Projectile {
    Vector2 center;
    Enemy target;
    float speed;
    float radius;
} Projectile;

typedef struct ProjectileList {
    Projectile projectiles[50];
    int qty_projectiles;
} ProjectileList;

typedef struct EnemyList {
    Enemy enemies[200];
    int qty_enemies;
} EnemyList;

// Menu & Settings
typedef struct Button {
    Rectangle rec;
    char text[120];
    Color rec_color, text_color;
} Button;

typedef struct Line {
    float x, y;
    float size;
    Color color;
} Line;

typedef enum GameScreen {
    MENU = 0,
    HISTORY = 1,
    GAMEPLAY = 2,
    SETTINGS = 3,
    CREDITS = 4,
    QUIT = 5,
    GAME_OVER = 6,
    PAUSE = 7
} GameScreen;

typedef struct Power_up {
    Rectangle rec;
    Texture2D texture;
    int type; // define qual o power_up
} Power_up;

typedef struct Power_up_list {
    Power_up power_up[200];
    int number_power_ups;
    int max_power_up;
} Power_up_list;

typedef struct Score {
    int points;
} Score;

typedef struct GameState {
    Player player;
    EnemyList enemy_list;
    ProjectileList projectile_list;
    Power_up_list power_up_list;
    Score score;
    double time_pass;
    int freeze;
} GameState;

typedef enum StateTyping {NOTLOCKED = 0, LOCKONENEMY = 1} StateTyping;

extern GameScreen currentScreen;
extern StateTyping state;
extern int current_wave;
extern bool wave_clear;
extern Score score;
extern int enemies_spawned_this_wave;

extern Sound botaoSound;
extern Sound morteSound;
extern Sound tiroSound;
extern Sound menuJogoSound;
extern Sound freezeSound;

extern Texture2D enemy_textures[3];
extern Texture2D background;
extern Texture2D background_menu;
extern Texture2D heart;

// Interface shared across screens; definitions live in main.c.
extern int paused;
extern int font_size;
extern Rectangle rec;
extern Button back;
extern Button back_menu;

// game.c
void DrawGame(Player* player, EnemyList* enemy_list, ProjectileList* projectile_list, Font myfont,Power_up_list power_up_list, Score* score);
void UpdateGameplay(Player* player, EnemyList* enemy_list, ProjectileList* projectile_list, double* time_pass, int *freeze, Power_up_list* power_up_list, Score* score);

// enemy.c
void InitTexts(void);
void UnloadEnemyTextures(void);
void ResetEnemyWaves(void);
void DrawEnemies(EnemyList* enemy_list, Font myfont, Texture2D enemyTextures[]);
void SpawnEnemy(EnemyList* enemy_list);
void UpdateEnemyWaves(EnemyList* enemy_list);
void MoveEnemies(EnemyList* enemy_list, Player* player, int* freeze, double* time_pass, Power_up_list* power_up_list, Score* score);
void RemoveEnemy(EnemyList* enemy_list, int index_enemy, Score* score, bool award_points);
void DelayEnemies(EnemyList* enemy_list);

// menu.c
void SetMenu(void);
void DrawMenu(void);
void UpdateMenu(void);

// settings.c
void SetSettings(void);
void DrawSettings(void);
void UpdateSettings(void);

// credits.c
void SetCredits(void);
void DrawCredits(void);
void UpdateCredits(void);

// pause.c
void SetPause(void);
void DrawPause(void);
void UpdatePause(GameState *game);

// game_over.c
void SetGameOver(void);
void DrawGameOver(void);
void UpdateGameOver(GameState *game);

// history.c
void DrawHistory(Texture2D background, Font myfont);
void UpdateHistory(void);

// player.c
void InitPlayerResources(void);
void CreatePlayer(Player* player);
void DrawPlayer(Player* player);
void TurnToEnemy(Player* player, EnemyList* enemy_list);
void UnloadPlayer(void);

// typing.c
void HandleTyping(Player* player, EnemyList* enemy_list, ProjectileList* projectile_list, int key_pressed);

// projectile.c
void UpdateProjectile(ProjectileList* projectile_list, EnemyList* enemy_list);
void DrawProjectiles(ProjectileList* projectile_list);
void CreateProjectile(ProjectileList* projectile_list, Player* player, EnemyList* enemy_list, int index_enemy);

// power_up.c
void Power_up_time(EnemyList* enemyList, double time_pass, float normal_speed, int* freeze); // função para congelar o tempo
void Power_up_delet(EnemyList* enemyList, int number_enemies, Score* score); // função que deleta X os inimigos
void Adicionar_power_up(int type, Power_up_list* power_up_list); // adiciona o power up a lista
void print_power_up_bar(Power_up_list power_up_list); // printa a barra com os power ups
void activate_power_up(Power_up_list* power_up_list, EnemyList* enemyList, double *time_pass, int* freeze, Score* score); // ativa o power up da barra
void Inicializar_power_up_list(Power_up_list* lista);
void UnloadPowerUpTextures(void); // inicializa a lista de power ups zerada

// lives.c
void InitLives(Player* player);
void LoseLife(Player* player);
void UpdateInvincibility(Player* player);
bool IsPlayerInvincible(Player* player);
void DrawLives(Player* player);
void DrawGameOver(void);
void UpdateGameOver(void);
void ResetGame(GameState* game);

// score.c
void DrawScore(Score* score, Font font);
void IncreaseScore(Score* score);
void InitScore(Score* score);

#endif
