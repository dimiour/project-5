#pragma once
#include <Arduino.h>

inline int16_t absolute_clamp(int16_t value, int16_t absolute_max) {
    if (value != 0) {
        if (abs(value) > abs(absolute_max)) {
            return value / abs(value) * absolute_max;
        }

        else return value;
    }

    else return 0;
}

inline int16_t easeOutQuad(uint8_t elapsed, uint8_t duration, int16_t start, int16_t target) {
    if (elapsed >= duration) return target;
    uint8_t t = (elapsed * 255) / duration;  // 0-255 normalized progress
    uint8_t ease = 255 - ((255 - t) * (255 - t) / 255);  // quadratic ease-out
    return start + (target - start) * ease / 255;
}