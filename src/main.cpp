#include <Arduboy2.h>
#include "entity.h"
#include "world.h"


Arduboy2 arduboy;

Entity player;
Entity enemies[NUM_ENEMIES];



int16_t playerEnergy = 100;
int16_t shotProgress = 0;




void setup() {
  // Initialize Arduboy
  arduboy.begin();

  player.x = 64;
  player.y = 32;
  player.size = 4;
  player.active = true;
  player.isPlayer = true;

  for (int i = 0; i < NUM_ENEMIES; i++) {
    enemies[i].x = 64;
    enemies[i].y = 32;
    enemies[i].velX = 0;
    enemies[i].velY = 0;
    enemies[i].size = 4;
    enemies[i].health = 1;
    enemies[i].isPlayer = false;
    enemies[i].active = false;
  }

  // Set the frame rate (optional, default is 60)
  arduboy.setFrameRate(60);
}

void resetPlayer() {
    player.health = 4;

    for (int i = 0; i < NUM_ENEMIES; i++) {
        enemies[i].active = false;
    }
}


uint8_t deathAnimationTimer = 0;
bool liveGame = false;
#define RESTART_COOLDOWN 160
bool checkLiveGame() {
    if (liveGame) return true;
    
    if (deathAnimationTimer < RESTART_COOLDOWN) {
        deathAnimationTimer++;
        liveGame = false;
        
    } else if (arduboy.anyPressed(0xff)) {
        arduboy.initRandomSeed();
        resetPlayer();
        liveGame = true;
    }

    return liveGame;
}

void endGame() {
    deathAnimationTimer = 0;
    liveGame = false;
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
        drawEntities();


    } else {
        // you can do something with the deathAnimationTimer

        if (deathAnimationTimer < RESTART_COOLDOWN) {
            arduboy.print("Revive in ");
            arduboy.print((RESTART_COOLDOWN - deathAnimationTimer) / 60 + 1);
        } else {
            arduboy.print("Press to start!");
        }

        drawEntities();
    }
    
    // Display the screen
    arduboy.display();
}
