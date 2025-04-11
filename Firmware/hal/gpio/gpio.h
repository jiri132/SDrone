#pragma once 

#include <stdint.h>
#include <stdbool.h>

void gpio_init(void);

void gpio_setpin(uint8_t pin);
void gpio_clearpin(uint8_t pin);
bool gpio_readpin(uint8_t pin);
void gpio_togglepin(uint8_t pin);