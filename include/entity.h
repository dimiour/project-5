#pragma once

#include <Arduboy2.h>
#include "utils.h"

extern Arduboy2 arduboy;


#define NUM_ENEMIES 4

struct Entity {
    int16_t x = 0, y = 0;
    int16_t velX = 0, velY = 0;
    uint8_t size = 4;
    bool active = false, isPlayer = false;

    Entity* enemies = nullptr;

    void draw() {
        if (!active) return;
        if (isPlayer) arduboy.fillCircle(x, y, size); 
        else arduboy.drawCircle(x, y, size);
    }
    
    void update(int8_t dx, int8_t dy) {
        // This takes user input (acceleration) and stores it in a dx/dy pair
            
        if (!active && arduboy.everyXFrames(random(isPlayer ? 500 : 200))) {
            x = random(128);
            y = random(64);
            size = isPlayer ? 4 : random(3, 7);
            active = true;
        }
        
        // This reduces the diagonal movement to 5/7 of the original speed
        if (dx != 0 && dy != 0) {
            dx = dx / 7 * 5;
            dy = dy / 7 * 5;
        }

        // Make it so if the velocity is less than 2 it jolts
        if (abs(velX) < 2) { dx *= 3; }
        if (abs(velY) < 2) { dy *= 3; }

        // This applies the acceleration to their velocity
        velX += dx;
        velY += dy;

        // This applies friction to the velocity
        velX *= isPlayer ? 0.90 : 0.92;
        velY *= isPlayer ? 0.90 : 0.92;
        
        // This applies the velocity to their position Y velocity-units / second
        x += absolute_clamp(velX, 1) * arduboy.everyXFrames(60 / abs(absolute_clamp(velX, 60)));
        y += absolute_clamp(velY, 1) * arduboy.everyXFrames(60 / abs(absolute_clamp(velY, 60)));
        
        // check if the entity is outside the (0,0,128,64) rect of the screen and flip the respective velocity and divide it by 0.90
        if (x < 0) { velX = -velX; x = 0; }
        if (x > 128) { velX = -velX; x = 128; }
        if (y < 0) { velY = -velY; y = 0; }
        if (y > 64) { velY = -velY; y = 64; }
        
    }
};

