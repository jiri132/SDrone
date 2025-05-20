#include <stdint.h>
#include "hal/gpio/gpio.h"
#include "hal/gpio/gpio_config.h"
#include "hal/spi/spi.h"  // Your register-based SPI3 HAL

#define SX1278_NSS   5
#define LED_DEBUG    16
#define LED_DEBUG2    4

void delay_ms(uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 40000; ++i);
}

void sx1278_select(void) {
    gpio_write(SX1278_NSS, GPIO_LOW);
}

void sx1278_deselect(void) {
    gpio_write(SX1278_NSS, GPIO_HIGH);
}

int app_main(void) {
    // Explicitly configure GPIOs using your API
    gpio_configure(&(gpio_config_t){ .pin = SX1278_NSS, .mode = GPIO_MODE_OUTPUT });
    gpio_configure(&(gpio_config_t){ .pin = LED_DEBUG, .mode = GPIO_MODE_OUTPUT });
    gpio_configure(&(gpio_config_t){ .pin = LED_DEBUG2, .mode = GPIO_MODE_OUTPUT });

    // Set initial states
    gpio_write(SX1278_NSS, GPIO_HIGH);     // CS idle high
    gpio_write(LED_DEBUG, GPIO_LOW);       // LED off
    gpio_write(LED_DEBUG, GPIO_LOW);       // LED off

    spi_init();  // Initialize SPI3 in mode 0

    delay_ms(100);

    while (1) {
        gpio_write(LED_DEBUG, GPIO_LOW);
        gpio_write(LED_DEBUG2, GPIO_LOW);

        sx1278_select();
        delay_ms(10);

        uint8_t version = sx1278_read_reg(0x42);
        
        delay_ms(10);
        sx1278_deselect();

        if (version == 0x12) {
            gpio_write(LED_DEBUG, GPIO_HIGH);
        }else {
            gpio_write(LED_DEBUG2, GPIO_HIGH);
        }

        delay_ms(10);
    }

    return 0;
}
