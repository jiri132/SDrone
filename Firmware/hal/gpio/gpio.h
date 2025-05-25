#pragma once

#include "gpio_types.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Configures a GPIO pin using the provided configuration structure.
 */
void gpio_configure_pin(const gpio_config_t* cfg);

/**
 * @brief Sets the digital level of the specified pin.
 */
void gpio_set_pin(uint8_t pin, uint8_t level);

/**
 * @brief Reads the current digital level of the specified pin.
 */
uint8_t gpio_read_pin(uint8_t pin);

/**
 * @brief Toggles the digital level of the specified pin.
 */
void gpio_toggle_pin(uint8_t pin);

/**
 * @brief Queues a pin for later state update (deferred write).
 */
void gpio_queue_pin_state(uint8_t pin, uint8_t level);

/**
 * @brief Removes a pin from the pending queue (no write will occur).
 */
void gpio_clear_queued_pin(uint8_t pin);

/**
 * @brief Applies all queued pin changes at once.
 */
void gpio_commit_queue(void);

/**
 * @brief Returns whether the pin is currently active/high.
 */
bool gpio_pin_active(uint8_t pin);
