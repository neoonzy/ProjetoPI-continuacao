#include "raylib.h"
#include "game.h"
#include "stdio.h"
#include <math.h>


void DrawProjectiles(ProjectileList* projectile_list) {
    int qty_projectiles = projectile_list->qty_projectiles; 
    for (int i = 0; i < qty_projectiles; i++) {
        Projectile projectile = projectile_list->projectiles[i];
        DrawCircle(projectile.center.x, projectile.center.y, projectile.radius, BLUE);
    }
}

void UpdateProjectile(ProjectileList* projectile_list, EnemyList* enemy_list) {
    float dx, dy, distance;
    for (int i = 0; i < projectile_list->qty_projectiles; i++) {
        Projectile projectile = projectile_list->projectiles[i];
        Enemy enemy;
        int index_enemy = -1;
        
        for (int j = 0; j < enemy_list->qty_enemies; j++) {
            if (strcmp(enemy_list->enemies[j].word, projectile.target.word) == 0) {
                enemy = enemy_list->enemies[j];
                index_enemy = j;
                break;
            }
        }

        Vector2 enemy_center = {enemy.rect.x + enemy.rect.width / 2, enemy.rect.y + enemy.rect.height / 2};
        dx = projectile.center.x - enemy_center.x;        
        dy = projectile.center.y - enemy_center.y;
        distance = sqrt(dx*dx + dy*dy);

        if (CheckCollisionCircleRec(projectile.center, projectile.radius, enemy.rect)) {
            for (int j = i; j < projectile_list->qty_projectiles; j++) {
                projectile_list->projectiles[j] = projectile_list->projectiles[j + 1];
            }
            projectile_list->qty_projectiles--;
            enemy_list->enemies[index_enemy].health--;
            enemy_list->enemies[index_enemy].delay_speed = 1.0;

        } else {
            
            projectile_list->projectiles[i].center.x += (-dx / distance) * projectile.speed;
            projectile_list->projectiles[i].center.y -= (dy / distance) * projectile.speed;
        }
    }
}

void CreateProjectile(ProjectileList* projectile_list, Player* player, EnemyList* enemy_list, int index_enemy) {
    Enemy enemy = enemy_list->enemies[index_enemy];
    Vector2 enemy_center = {enemy.rect.x + enemy.rect.width / 2, enemy.rect.y - enemy.rect.height / 2};
    
    float dx = player->rect.x - enemy_center.x;
    float dy = player->rect.y - enemy_center.y;

    Projectile projectile;
    projectile.center = (Vector2) {player->rect.x - ((dx / dy) * 30), player->rect.y - 30};
    projectile.radius = 4;
    projectile.target = enemy_list->enemies[index_enemy];
    projectile.speed = 14;
    
    int qty_projectiles = projectile_list->qty_projectiles;
    projectile_list->projectiles[qty_projectiles] = projectile;
    projectile_list->qty_projectiles++;
    PlaySound(tiroSound); // Toca o som de tiro
}