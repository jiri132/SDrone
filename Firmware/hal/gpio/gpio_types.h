#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum gpio_mode {
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_INPUT_PULLUP,
    GPIO_MODE_INPUT_PULLDOWN,
    GPIO_MODE_ALT_FUNC, // (optional)
} gpio_mode_e;

typedef struct gpio_config {
    uint8_t pin;
    gpio_mode_e mode;
    bool open_drain;
    uint8_t function; // optional if you want alternate functions later
} gpio_config_t;


