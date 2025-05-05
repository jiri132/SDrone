#include <stdint.h>
#include "hal/gpio/gpio.h"
#include "hal/spi2/spi2.h"


#define SX1278_NSS   5   // Default VSPI CS pin
#define LED_DEBUG   16   // Example debug LED pin

#define DPORT_BASE                0x3FF00000
#define DPORT_PERIP_CLK_EN_REG    (*(volatile uint32_t*)(DPORT_BASE + 0x0C0))
#define DPORT_PERIP_RST_EN_REG    (*(volatile uint32_t*)(DPORT_BASE + 0x0C4))

#define DPORT_SPI2_CLK_EN         (1 << 6) // For HSPI
#define DPORT_SPI3_CLK_EN         (1 << 16) // For VSPI
#define DPORT_SPI2_RST            (1 << 6)
#define DPORT_SPI3_RST            (1 << 16)

void enable_spi_clock(spi_perif_e spi_perif) {
    if (spi_perif == SPI_PERIF_HSPI) {
        DPORT_PERIP_CLK_EN_REG |= DPORT_SPI2_CLK_EN;
        DPORT_PERIP_RST_EN_REG &= ~DPORT_SPI2_RST;
    } else if (spi_perif == SPI_PERIF_VSPI) {
        DPORT_PERIP_CLK_EN_REG |= DPORT_SPI3_CLK_EN;
        DPORT_PERIP_RST_EN_REG &= ~DPORT_SPI3_RST;
    }
}

void delay_ms(uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 40000; ++i);
}

int app_main(void) {
    // Setup debug LED (GPIO16)
    gpio_config_t led_cfg = {
        .pin = LED_DEBUG,
        .mode = GPIO_MODE_OUTPUT
    };
    gpio_configure(&led_cfg);
    gpio_configure(&(gpio_config_t){ .pin = 15, .mode = GPIO_MODE_OUTPUT });
    gpio_write(15, 1); // force HIGH
    
    // Enable SPI3 (VSPI) peripheral
    enable_spi_clock(SPI_PERIF_VSPI);

    // Initialize VSPI in Mode 0 with slow clock (≈ 39 kHz)
    spi_init(SPI_MODE_0, 0x7FF, SPI_PERIF_VSPI);

    delay_ms(200); // Optional startup delay

    while (1) {

        // Send address to read RegVersion (0x42)
        spi_buffer_data(0x42000000); // Address phase
        spi_buffer_data(0x00000000); // Dummy read phase

        gpio_write(15, 0); // force HIGH

        delay_ms(50);

        spi_transaction(); // Start full-duplex transaction
 
        delay_ms(50);

        gpio_write(15, 1); // force HIGH

        // Extract high byte of second 32-bit received word
        uint8_t version = (uint8_t)((spi_received(1) >> 24) & 0xFF);

        // Blink if version is correct (0x12 for SX1278)
        if (version == 0x0) {
            gpio_write(LED_DEBUG, 1);
            delay_ms(100);
            gpio_write(LED_DEBUG, 0);
        }

        delay_ms(500);
    }

    return 0;
}
