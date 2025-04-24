#pragma once 

#include "gpio_types.h"
#include <stdint.h>

void gpio_configure(const gpio_config_t* cfg);
void gpio_write(uint8_t pin, uint8_t level);
uint8_t gpio_read(uint8_t pin);
void gpio_toggle(uint8_t pin);