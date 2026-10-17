#include "game.h"

GameScreen currentScreen = MENU;
GameState game = {0};

int paused = 0;
int font_size = 50;
Rectangle rec = {(SCREEN_WIDTH - 400)/2, (SCREEN_HEIGHT - 80)/2, 400, 80};
Button back = {{(SCREEN_WIDTH - 33*5)/2, SCREEN_HEIGHT/2 + 250, 33*5, 50}, "Voltar", BLACK, RAYWHITE};
Button back_menu = {{(SCREEN_WIDTH - 400)/2, SCREEN_HEIGHT/2 + 180, 400, 80}, "Menu", RAYWHITE, BLACK};

Sound morteSound, menuJogoSound, tiroSound, botaoSound, freezeSound;
Texture2D enemy_textures[3], background, background_menu, heart;
static Texture2D background_history;
static Font myfont;

static void UpdateGame(void);
static void DrawFrame(void);

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CInType");
    InitAudioDevice();
    SetTargetFPS(60);
    InitPlayerResources();
    CreatePlayer(&game.player);
    InitScore(&game.score);
    InitTexts();
    Inicializar_power_up_list(&game.power_up_list);
    myfont = LoadFont("COUR.TTF");
    background_history = LoadTexture("sprites/background_history.png");
    morteSound = LoadSound("sounds/morte.mp3");
    menuJogoSound = LoadSound("sounds/menu_jogo.mp3");
    tiroSound = LoadSound("sounds/tiro.mp3");
    botaoSound = LoadSound("sounds/botao.mp3");
    freezeSound = LoadSound("sounds/freeze.mp3");
    enemy_textures[0] = LoadTexture("sprites/inimigo1.png");
    enemy_textures[1] = LoadTexture("sprites/inimigo2.png");
    enemy_textures[2] = LoadTexture("sprites/inimigo3.png");
    background = LoadTexture("sprites/background.png");
    background_menu = LoadTexture("sprites/backgroundmenu.png");
    heart = LoadTexture("sprites/heart.png");
    SetSettings();

    while (!WindowShouldClose() && currentScreen != QUIT) {
        UpdateGame();
        DrawFrame();
    }

    UnloadSound(morteSound);
    UnloadSound(menuJogoSound);
    UnloadSound(tiroSound);
    UnloadSound(botaoSound);
    UnloadSound(freezeSound);
    UnloadEnemyTextures();
    UnloadPowerUpTextures();
    UnloadTexture(background);
    UnloadTexture(background_menu);
    UnloadTexture(background_history);
    UnloadTexture(heart);
    UnloadPlayer();
    UnloadFont(myfont);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

static void UpdateGame(void) {
    if (!IsSoundPlaying(menuJogoSound)) PlaySound(menuJogoSound);
    switch (currentScreen) {
        case MENU: UpdateMenu(); break;
        case GAMEPLAY:
            UpdateGameplay(&game.player, &game.enemy_list, &game.projectile_list,
                           &game.time_pass, &game.freeze, &game.power_up_list, &game.score);
            break;
        case HISTORY: UpdateHistory(); break;
        case PAUSE: UpdatePause(&game); break;
        case GAME_OVER: UpdateGameOver(&game); break;
        case SETTINGS: UpdateSettings(); break;
        case CREDITS: UpdateCredits(); break;
        case QUIT: break;
    }
}

static void DrawFrame(void) {
    BeginDrawing();
    switch (currentScreen) {
        case MENU: DrawMenu(); break;
        case GAMEPLAY:
            DrawGame(&game.player, &game.enemy_list, &game.projectile_list,
                     myfont, game.power_up_list, &game.score);
            break;
        case HISTORY: DrawHistory(background_history, myfont); break;
        case PAUSE: DrawPause(); break;
        case GAME_OVER: DrawGameOver(); break;
        case SETTINGS: DrawSettings(); break;
        case CREDITS: DrawCredits(); break;
        case QUIT: ClearBackground(BLACK); break;
    }
    EndDrawing();
}
