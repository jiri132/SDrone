#pragma once

#include "esp_err.h"
#include "esp_intr_alloc.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Interrupt handler function pointer type.
 * 
 * The function will be called from the ISR context.
 * Keep it fast and avoid any blocking operations.
 */
typedef void (*irq_handler_t)(void *arg);

/**
 * @brief IRQ trigger types for GPIO pins.
 * 
 * These map directly to ESP32 hardware-supported GPIO interrupt types.
 * Used when attaching an interrupt to a specific GPIO pin.
 */
typedef enum gpio_irq_type {
    GPIO_INT_NONE = 0,         ///< No interrupt
    GPIO_INT_LOW_LEVEL,        ///< Level triggered on logic low
    GPIO_INT_HIGH_LEVEL,       ///< Level triggered on logic high
    GPIO_INT_FALLING_EDGE,     ///< Interrupt on falling edge
    GPIO_INT_RISING_EDGE,      ///< Interrupt on rising edge
    GPIO_INT_ANY_EDGE          ///< Interrupt on any edge (rising or falling)
} gpio_irq_type_t;

/**
 * @brief Attach an interrupt handler to a peripheral IRQ source.
 * 
 * This function registers a handler to any interrupt source (e.g., SPI, UART, GPIO matrix).
 * Uses ESP-IDF's `esp_intr_alloc()` internally to map the source to a CPU interrupt line.
 * 
 * @param irq_source The interrupt source (e.g., ETS_GPIO_INTR_SOURCE).
 * @param handler    The ISR function to call.
 * @param arg        User-defined argument passed to the handler.
 * @param flags      Allocation flags (e.g., ESP_INTR_FLAG_LEVEL1, ESP_INTR_FLAG_IRAM).
 * 
 * @return esp_err_t ESP_OK on success, error code on failure.
 */
esp_err_t irq_attach(int irq_source, irq_handler_t handler, void *arg, int flags);

/**
 * @brief Detach (free) an interrupt handler by source.
 * 
 * If a handler was previously attached via irq_attach(), this function
 * will remove and free it. You can safely call this even if no handler
 * is registered for the source — it will return ESP_ERR_INVALID_STATE.
 * 
 * @param irq_source The same peripheral interrupt source passed to irq_attach().
 * 
 * @return esp_err_t ESP_OK on success, ESP_ERR_INVALID_ARG or other on error.
 */
esp_err_t irq_detach(int irq_source);

/**
 * @brief Attach an interrupt to a specific GPIO pin.
 * 
 * This sets up a hardware-level interrupt on the given GPIO pin.
 * It will automatically install the ISR service once (on first call),
 * and assign the ISR via `gpio_isr_handler_add()`.
 * 
 * @param gpio     GPIO pin number (0–39).
 * @param type     Type of trigger (edge or level).
 * @param handler  ISR to execute on the event.
 * @param arg      User argument passed to the ISR.
 * 
 * @return esp_err_t ESP_OK on success, error code otherwise.
 */
esp_err_t irq_attach_gpio(uint8_t gpio, gpio_irq_type_t type, irq_handler_t handler, void *arg);

/**
 * @brief Detach (remove) an interrupt from a GPIO pin.
 * 
 * This unbinds the previously registered ISR from the given GPIO.
 * Safe to call even if no handler is installed.
 * 
 * @param gpio GPIO pin number (0–39).
 */
void irq_detach_gpio(uint8_t gpio);
