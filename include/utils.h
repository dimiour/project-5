#pragma once
#include <Arduino.h>

int16_t absolute_clamp(int16_t value, int16_t absolute_max) {
    if (value != 0) {
        if (abs(value) > abs(absolute_max)) {
            return value / abs(value) * absolute_max;
        }
        
        else return value;
    }

    else return 0;
}