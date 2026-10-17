#include "game.h"
#include <math.h>

void DrawProjectiles(ProjectileList* list) {
    for (int i = 0; i < list->qty_projectiles; i++) {
        Projectile *p = &list->projectiles[i];
        DrawCircle((int)p->center.x, (int)p->center.y, p->radius, BLUE);
    }
}

static void RemoveProjectile(ProjectileList* list, int index) {
    for (int j = index; j + 1 < list->qty_projectiles; j++)
        list->projectiles[j] = list->projectiles[j + 1];
    list->qty_projectiles--;
}

void UpdateProjectile(ProjectileList* list, EnemyList* enemies) {
    int i = 0;
    while (i < list->qty_projectiles) {
        Projectile *p = &list->projectiles[i];
        int index = -1;
        for (int j = 0; j < enemies->qty_enemies; j++) {
            if (enemies->enemies[j].id == p->target_id) { index = j; break; }
        }
        if (index < 0) { RemoveProjectile(list, i); continue; }
        Enemy *target = &enemies->enemies[index];
        Vector2 center = {target->rect.x + target->rect.width/2,
                          target->rect.y + target->rect.height/2};
        float dx = center.x - p->center.x;
        float dy = center.y - p->center.y;
        float distance = sqrtf(dx * dx + dy * dy);
        if (distance > 0.0001f) {
            float travel = p->speed * GetFrameTime() * 60.0f;
            float factor = fminf(travel / distance, 1.0f);
            p->center.x += dx * factor;
            p->center.y += dy * factor;
        }
        if (CheckCollisionCircleRec(p->center, p->radius, target->rect)) {
            if (target->health > 0) target->health--;
            target->delay_speed = 1.0f;
            RemoveProjectile(list, i);
        } else i++;
    }
}

void CreateProjectile(ProjectileList* list, Player* player, EnemyList* enemies, int index) {
    if (index < 0 || index >= enemies->qty_enemies ||
        list->qty_projectiles >= (int)(sizeof(list->projectiles)/sizeof(list->projectiles[0]))) return;
    Projectile p = {0};
    p.center = (Vector2){player->rect.x + player->rect.width/2,
                         player->rect.y + player->rect.height/2 - 30.0f};
    p.radius = 4.0f;
    p.target_id = enemies->enemies[index].id;
    p.speed = 14.0f;
    list->projectiles[list->qty_projectiles++] = p;
    PlaySound(tiroSound);
}
