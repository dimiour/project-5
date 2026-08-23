#pragma once
#include "entity.h"

extern Arduboy2 arduboy;
extern Entity player;
extern Entity enemies[NUM_ENEMIES];
extern void endGame();

#define PLAYER_ACCELERATION 7

inline void updatePlayer() {
    int16_t dx = 0, dy = 0;

    if (arduboy.pressed(UP_BUTTON)) { dy += -PLAYER_ACCELERATION; }
    if (arduboy.pressed(DOWN_BUTTON)) { dy += PLAYER_ACCELERATION; }
    if (arduboy.pressed(LEFT_BUTTON)) { dx += -PLAYER_ACCELERATION; }
    if (arduboy.pressed(RIGHT_BUTTON)) { dx += PLAYER_ACCELERATION; }

    player.update(dx, dy);

    arduboy.print(player.health);

    if (player.health == 0) endGame();
}

inline bool checkCollision(Entity& a, Entity& b) {
    if (a.active && b.active) {
        int16_t cx = a.x - b.x;
        int16_t cy = a.y - b.y;
        int16_t cs = a.size + b.size;

        if (cx * cx + cy * cy < cs * cs) {
            a.velX += cx * abs(cx) - cs;
            a.velY += cy * abs(cy) - cs;

            b.velX -= cx * abs(cx) - cs;
            b.velY -= cy * abs(cy) - cs;

            return true;
        }
    }

    return false;
}

#define ENEMY_ACCELERATION 6

inline void updateEnemies() {
    for (int i = 0; i < NUM_ENEMIES; i++) {


        int16_t dx = 0, dy = 0;
        if (player.y - enemies[i].y < -10) dy += -ENEMY_ACCELERATION;
        if (player.y - enemies[i].y > 10) dy += ENEMY_ACCELERATION;
        if (player.x - enemies[i].x < -10) dx += -ENEMY_ACCELERATION;
        if (player.x - enemies[i].x > 10) dx += ENEMY_ACCELERATION;

        for (int j = i + 1; j < NUM_ENEMIES; j++) {
            checkCollision(enemies[i], enemies[j]);
        }

        if (checkCollision(enemies[i], player)) {
            enemies[i].health--;
            player.health--;
        }

        enemies[i].update(dx, dy);

        if (enemies[i].health == 0) {
            enemies[i].active = false;
            enemies[i].health = 1;
        }
    }
}

inline void drawEntities() {
    player.draw();
    for (int i = 0; i < NUM_ENEMIES; i++) {
        enemies[i].draw();
    }
}
