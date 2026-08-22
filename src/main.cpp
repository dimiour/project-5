#include <Arduboy2.h>
#include "utils.h"

Arduboy2 arduboy;

int16_t playerX = 64;
int16_t playerY = 32;
int8_t playerVelX = 0;
int8_t playerVelY = 0;

struct Enemy {
  int16_t x;
  int16_t y;
  int8_t velX;
  int8_t velY;
  bool active;
};

Enemy enemies[10];

void setup() {
  // Initialize Arduboy
  arduboy.begin();

  arduboy.initRandomSeed();

  for (int i = 0; i < 10; i++) {
      enemies[i].x = random(128);
      enemies[i].y = random(64);
      enemies[i].velX = 0;
      enemies[i].velY = 0;
      enemies[i].active = false;
  }

  // Set the frame rate (optional, default is 60)
  arduboy.setFrameRate(60);
}




void loop() {
    // Pause rendering until it's time for the next frame
    if (!arduboy.nextFrame())
        return;
    
    // Clear the display
    arduboy.clear();
    
    // This takes user input and stores it in a dx/dy pair
    int8_t dx = 0, dy = 0;
    if (arduboy.pressed(UP_BUTTON)) { dy = -7; }
    if (arduboy.pressed(DOWN_BUTTON)) { dy = 7; }
    if (arduboy.pressed(LEFT_BUTTON)) { dx = -7; }
    if (arduboy.pressed(RIGHT_BUTTON)) { dx = 7; }
    
    // This reduces the diagonal movement to 5/7 of the original speed
    if (dx != 0 && dy != 0) {
        dx = dx / 7 * 5;
        dy = dy / 7 * 5;
    }

    // Make it so if the velocity is less than 2 it jolts
    if (abs(playerVelX) < 2) { dx *= 3; }
    if (abs(playerVelY) < 2) { dy *= 3; }
    
    // This applies the input to the player's velocity
    playerVelX += dx;
    playerVelY += dy;

    for (int i = 0; i < 10; i++) {
        if (enemies[i].active) {
    
            int8_t dx = 0, dy = 0;
            if (playerY - enemies[i].y < -10) { dy = -3; }
            if (playerY - enemies[i].y > 10) { dy = 3; }
            if (playerX - enemies[i].x < -10) { dx = -3; }
            if (playerX - enemies[i].x > 10) { dx = 3; }
        
            // This reduces the diagonal movement to 5/7 of the original speed
            if (dx != 0 && dy != 0) {
                dx = dx / 3 * 2;
                dy = dy / 3 * 2;
            }
        
            enemies[i].velX += dx;
            enemies[i].velY += dy;

            uint8_t check = random(10);
            if (abs(enemies[i].velX - enemies[check].velX) < 10 && abs(enemies[i].velY - enemies[check].velY) < 10) {
                enemies[i].velX -= enemies[check].velX + random(-10, 10);
                enemies[i].velY -= enemies[check].velY + random(-10, 10);
            }
        
            enemies[i].velX *= 0.97;
            enemies[i].velY *= 0.97;
        
            enemies[i].x += absolute_clamp(enemies[i].velX, 1) * arduboy.everyXFrames(60 / abs(absolute_clamp(enemies[i].velX, 60)));
            enemies[i].y += absolute_clamp(enemies[i].velY, 1) * arduboy.everyXFrames(60 / abs(absolute_clamp(enemies[i].velY, 60)));
        
        
            if (enemies[i].x < 0) { enemies[i].x = 0; }
            if (enemies[i].x > 128) { enemies[i].x = 128; }
            if (enemies[i].y < 0) { enemies[i].y = 0; }
            if (enemies[i].y > 64) { enemies[i].y = 64; }
        } else {
            if (arduboy.everyXFrames(random(200))) {
                enemies[i].x = random(128);
                enemies[i].y = random(64);
                enemies[i].active = true;
            }
        }

        if (enemies[i].active) {
            arduboy.drawPixel(enemies[i].x, enemies[i].y);
        }
    }
    // This applies friction to the player's velocity
    playerVelX *= 0.90;
    playerVelY *= 0.90;
    
    // This applies the player's velocity to their position Y velocity-units / second
    playerX += absolute_clamp(playerVelX, 1) * arduboy.everyXFrames(60 / abs(absolute_clamp(playerVelX, 60)));
    playerY += absolute_clamp(playerVelY, 1) * arduboy.everyXFrames(60 / abs(absolute_clamp(playerVelY, 60)));
    
    // check if the player is outside the (0,0,128,64) rect of the screen and flip the respective velocity and divide it by 0.90
    if (playerX < 0) { playerVelX = -playerVelX; playerX = 0; }
    if (playerX > 128) { playerVelX = -playerVelX; playerX = 128; }
    if (playerY < 0) { playerVelY = -playerVelY; playerY = 0; }
    if (playerY > 64) { playerVelY = -playerVelY; playerY = 64; }
    
    // Draw game elements here
    arduboy.drawCircle(playerX, playerY, 3);
    
    // Display the screen
    arduboy.display();

    
}
