/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int LED = 18; 
const int BTN = 14;

const int COIL1_PIN = 20;  // IN1 no ULN2003
const int COIL2_PIN = 21;  // IN2 no ULN2003
const int COIL3_PIN = 22;  // IN3 no ULN2003
const int COIL4_PIN = 26;  // IN4 no ULN2003

const float STEP_ANGLE = 0.17 * 4; 
const int STEPS_PER_REVOLUTION = (int)(360.0 / STEP_ANGLE);

void step_motor(int steps) {
    int step_sequence[8][4] = {
        {1, 0, 0, 0},  
        {1, 1, 0, 0},  
        {0, 1, 0, 0}, 
        {0, 1, 1, 0}, 
        {0, 0, 1, 0}, 
        {0, 0, 1, 1}, 
        {0, 0, 0, 1}, 
        {1, 0, 0, 1}  
    };

    for (int i = 0; i < steps; i++) {
        for (int step = 7; step >= 0; step--) { 
            gpio_put(COIL1_PIN, step_sequence[step][0]);
            gpio_put(COIL2_PIN, step_sequence[step][1]);
            gpio_put(COIL3_PIN, step_sequence[step][2]);
            gpio_put(COIL4_PIN, step_sequence[step][3]);
            sleep_us(1000);  
        }
    }
}

int main() {
    stdio_init_all();

    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);

    gpio_init(BTN);
    gpio_set_dir(BTN, GPIO_IN);
    gpio_pull_up(BTN);

    gpio_init(COIL1_PIN);
    gpio_set_dir(COIL1_PIN, GPIO_OUT);

    gpio_init(COIL2_PIN);
    gpio_set_dir(COIL2_PIN, GPIO_OUT);

    gpio_init(COIL3_PIN);
    gpio_set_dir(COIL3_PIN, GPIO_OUT);

    gpio_init(COIL4_PIN);
    gpio_set_dir(COIL4_PIN, GPIO_OUT);

    while (true) {
        if (!gpio_get(BTN)) {
            gpio_put(LED, 1);
            step_motor(STEPS_PER_REVOLUTION);  
            gpio_put(LED, 0);

            while (!gpio_get(BTN)) {
                sleep_ms(10);
            }
        }
    }
}
