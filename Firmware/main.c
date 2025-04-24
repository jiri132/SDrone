#include <stdint.h>
#include "hal/gpio/gpio.h"

// --- Delay Function ---
void delay(volatile int cycles) {
    while (cycles--);
}

// --- Main ---
void app_main(void) {
    
    gpio_config_t test_pin = {
        .pin = 16,
        .mode = GPIO_MODE_OUTPUT
    };

    gpio_configure(&test_pin); // Your HAL call to configure the pin

    while (1) {
        gpio_write(16, 1);

        delay(1000000);

        gpio_write(16, 0);

        delay(1000000);
    }
}
