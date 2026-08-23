#include <Arduboy2.h>
#include "Arduboy2Core.h"
#include "entity.h"

Arduboy2 arduboy;

Entity player;
Entity enemies[NUM_ENEMIES];

void setup() {
  // Initialize Arduboy
  arduboy.begin();

  player.x = 64;
  player.y = 32;
  player.velX = 0;
  player.velY = 0;
  player.size = 4;
  player.active = true;
  player.isPlayer = true;

  for (int i = 0; i < NUM_ENEMIES; i++) {
    enemies[i].x = 64;
    enemies[i].y = 32;
    enemies[i].velX = 0;
    enemies[i].velY = 0;
    enemies[i].size = 4;
    enemies[i].isPlayer = false;
    enemies[i].active = false;
  }

  // Set the frame rate (optional, default is 60)
  arduboy.setFrameRate(60);
}

uint8_t deathAnimationTimer = 0;
bool liveGame = false;
#define RESTART_COOLDOWN 100
bool checkLiveGame() {
    if (liveGame) return true;
    
    if (deathAnimationTimer < RESTART_COOLDOWN) {
        deathAnimationTimer++;
        liveGame = false;
    } else if (arduboy.anyPressed(0xff)) {
        arduboy.initRandomSeed();
        liveGame = true;
    }

    return liveGame;
}

void endGame() {
    deathAnimationTimer = 0;
    liveGame = false;
}

#define PLAYER_ACCELERATION 7

void updatePlayer() {
    int8_t dx = 0, dy = 0;
    
    if (arduboy.pressed(UP_BUTTON)) { dy += -PLAYER_ACCELERATION; }
    if (arduboy.pressed(DOWN_BUTTON)) { dy += PLAYER_ACCELERATION; }
    if (arduboy.pressed(LEFT_BUTTON)) { dx += -PLAYER_ACCELERATION; }
    if (arduboy.pressed(RIGHT_BUTTON)) { dx += PLAYER_ACCELERATION; }
    
    player.update(dx, dy);

    
    
    player.draw();

    if (!player.active) endGame();
}

bool checkCollision(Entity& a, Entity& b) {
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

#define ENEMY_ACCELERATION 5

void updateEnemies() {
    for (int i = 0; i < NUM_ENEMIES; i++) {
        
        
        int8_t dx = 0, dy = 0;
        if (player.y - enemies[i].y < -10) { dy += -ENEMY_ACCELERATION; }
        if (player.y - enemies[i].y > 10) { dy += ENEMY_ACCELERATION; }
        if (player.x - enemies[i].x < -10) { dx += -ENEMY_ACCELERATION; }
        if (player.x - enemies[i].x > 10) { dx += ENEMY_ACCELERATION; }

        for (int j = i + 1; j < NUM_ENEMIES; j++) {
            checkCollision(enemies[i], enemies[j]);
        }

        checkCollision(enemies[i], player);
        
        enemies[i].update(dx, dy);

       

        if (enemies[i].active) {
            enemies[i].draw();
        }
    }
}

void loop() {
    // Pause rendering until it's time for the next frame
    if (!arduboy.nextFrame())
        return;
    
    // Clear the display
    arduboy.clear();

    if (checkLiveGame()) {
        updatePlayer();
        updateEnemies();
    } else {
        // you can do something with the deathAnimationTimer
        arduboy.print(deathAnimationTimer);
    }
    
    // Display the screen
    arduboy.display();
}
