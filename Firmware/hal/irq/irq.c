#include "irq.h"
#include "driver/gpio.h"
#include "esp_intr_alloc.h"

#define MAX_IRQ_SOURCES 80
#define MAX_GPIO_IRQS   40

// Lookup table for peripheral interrupt handles
static intr_handle_t irq_handles[MAX_IRQ_SOURCES] = {0};

// One-time initialization flag for GPIO ISR service
static bool gpio_isr_service_initialized = false;


/* -------------------------------------------------------------------------- */
/*                         GENERIC PERIPHERAL INTERRUPTS                      */
/* -------------------------------------------------------------------------- */

esp_err_t irq_attach(int irq_source, irq_handler_t handler, void *arg, int flags) {
    if (irq_source < 0 || irq_source >= MAX_IRQ_SOURCES || handler == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    // Free existing handler if already attached
    if (irq_handles[irq_source]) {
        esp_intr_free(irq_handles[irq_source]);
        irq_handles[irq_source] = NULL;
    }

    return esp_intr_alloc(irq_source, flags, handler, arg, &irq_handles[irq_source]);
}

esp_err_t irq_detach(int irq_source) {
    if (irq_source < 0 || irq_source >= MAX_IRQ_SOURCES) {
        return ESP_ERR_INVALID_ARG;
    }

    if (!irq_handles[irq_source]) {
        return ESP_ERR_INVALID_STATE;
    }

    esp_err_t err = esp_intr_free(irq_handles[irq_source]);
    irq_handles[irq_source] = NULL;
    return err;
}


/* -------------------------------------------------------------------------- */
/*                                GPIO INTERRUPTS                             */
/* -------------------------------------------------------------------------- */

// Maps our enum to ESP-IDF GPIO interrupt types
static int get_gpio_intr_type(gpio_irq_type_t type) {
    switch (type) {
        case GPIO_INT_LOW_LEVEL:    return GPIO_INTR_LOW_LEVEL;
        case GPIO_INT_HIGH_LEVEL:   return GPIO_INTR_HIGH_LEVEL;
        case GPIO_INT_FALLING_EDGE: return GPIO_INTR_NEGEDGE;
        case GPIO_INT_RISING_EDGE:  return GPIO_INTR_POSEDGE;
        case GPIO_INT_ANY_EDGE:     return GPIO_INTR_ANYEDGE;
        case GPIO_INT_NONE:
        default:                    return GPIO_INTR_DISABLE;
    }
}

esp_err_t irq_attach_gpio(uint8_t gpio, gpio_irq_type_t type, irq_handler_t handler, void *arg) {
    if (!handler || gpio >= MAX_GPIO_IRQS) {
        return ESP_ERR_INVALID_ARG;
    }

    // Only install the ISR service once
    if (!gpio_isr_service_initialized) {
        esp_err_t err = gpio_install_isr_service(0); // Default config, no flags
        if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) return err;
        gpio_isr_service_initialized = true;
    }

    // Set GPIO to input mode with given interrupt trigger
    gpio_set_intr_type(gpio, get_gpio_intr_type(type));
    gpio_isr_handler_add(gpio, handler, arg);  // Add user ISR

    return ESP_OK;
}

void irq_detach_gpio(uint8_t gpio) {
    if (gpio < MAX_GPIO_IRQS) {
        gpio_isr_handler_remove(gpio);  // Removes the handler
        // Note: no need to uninstall service — it may be reused
    }
}
