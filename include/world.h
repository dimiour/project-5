#pragma once
#include "entity.h"
#include "utils.h"

extern Arduboy2 arduboy;
extern Entity player;
extern Entity enemies[NUM_ENEMIES];
extern void endGame();


extern int16_t playerEnergy;
extern int16_t shotProgress;
#define SHOT_RANGE 10
#define SHOT_DURATION 20
#define SHOT_ENERGY 50

#define PLAYER_ACCELERATION 7

inline void updatePlayer() {
    int16_t dx = 0, dy = 0;

    if (arduboy.pressed(UP_BUTTON)) { dy += -PLAYER_ACCELERATION; }
    if (arduboy.pressed(DOWN_BUTTON)) { dy += PLAYER_ACCELERATION; }
    if (arduboy.pressed(LEFT_BUTTON)) { dx += -PLAYER_ACCELERATION; }
    if (arduboy.pressed(RIGHT_BUTTON)) { dx += PLAYER_ACCELERATION; }

    if (arduboy.pressed(A_BUTTON) && shotProgress == 0 && playerEnergy >= SHOT_ENERGY) {
        shotProgress = 1; playerEnergy -= SHOT_ENERGY; 
    }
    
    if (shotProgress > 0) shotProgress++;
    if (shotProgress >= SHOT_DURATION) shotProgress = 0;
    
    arduboy.drawCircle(player.x, player.y, easeOutQuad(shotProgress, SHOT_DURATION, 0, SHOT_RANGE));



    arduboy.print(player.health);
    arduboy.print(" Health, ");

    if (playerEnergy < 100 && arduboy.everyXFrames(3)) playerEnergy++;
    arduboy.print(playerEnergy);
    arduboy.print(" Energy");

    player.update(dx, dy);
    if (player.health == 0) endGame();
}

inline bool checkCollision(Entity& a, Entity& b) {
    if (a.active && b.active) {
        int16_t cx = a.x - b.x;
        int16_t cy = a.y - b.y;
        
        int16_t cs = a.size + b.size + (b.isPlayer ? shotProgress : 0);

        if (cx * cx + cy * cy < cs * cs) {
            a.velX += cx * abs(cx) - cs;
            a.velY += cy * abs(cy) - cs;

            if (b.isPlayer && shotProgress > 0) {
                if (a.health > 0 && !a.hitMarked) {
                    a.health--;
                    a.hitMarked = true;
                }
            } else {
                b.velX -= cx * abs(cx) - cs;
                b.velY -= cy * abs(cy) - cs;

                if (b.isPlayer && shotProgress == 0 && b.health > 0) b.health--;
            }

            return true;
        } else if (a.hitMarked) {
            a.hitMarked = false;
        }
    }

    return false;
}

#define ENEMY_ACCELERATION 6
#define REVIVE_DURATION 100

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

        checkCollision(enemies[i], player);

        enemies[i].update(dx, dy);

        if (enemies[i].health == 0) {
            enemies[i].active = false;
            enemies[i].reviveCountdown = REVIVE_DURATION;
            enemies[i].health = 1;
        }
    }
}

inline void resetPlayer() {
    player.health = 4;

    for (int i = 0; i < NUM_ENEMIES; i++) {
        enemies[i].active = false;
        enemies[i].reviveCountdown = random(REVIVE_DURATION, REVIVE_DURATION * 2);
        enemies[i].health = 1;
        enemies[i].size = random(3, 5);
    }
}

inline void drawEntities() {
    player.draw();
    for (int i = 0; i < NUM_ENEMIES; i++) {
        enemies[i].draw();
    }
}
