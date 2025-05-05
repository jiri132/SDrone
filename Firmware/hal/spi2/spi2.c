#include "spi2.h"
#include <stdint.h>
#include <stdbool.h>

static volatile uint32_t* SPI_BASE = 0x3FF65000;

static uint32_t spi_tx_buffer[MAX_SPI_WORDS];
static uint32_t spi_rx_buffer[MAX_SPI_WORDS];
static uint8_t spi_buffer_count = 0;
static bool spi_initialized = false;

// --- Register Offsets (for SPI3_BASE = 0x3FF65000) ---
#define SPI_CMD_REG         (*(volatile uint32_t*)(SPI_BASE))
#define SPI_USER_REG        (*(volatile uint32_t*)(SPI_BASE + 0x1C))
#define SPI_CLOCK_REG       (*(volatile uint32_t*)(SPI_BASE + 0x18))
#define SPI_W_REG(n)        (*(volatile uint32_t*)(SPI_BASE + 0x80 + (n) * 4))
#define SPI_PIN_REG         (*(volatile uint32_t*)(SPI_BASE + 0x34))

#define SPI_USR             (1 << 18)
#define SPI_USR_MOSI        (1 << 27)
#define SPI_USR_MISO        (1 << 28)
#define SPI_USR_MOSI_HIGHPART (1 << 25)
#define SPI_USR_MISO_HIGHPART (1 << 24)
#define SPI_CS_SETUP        (1 << 5)
#define SPI_CS_HOLD         (1 << 4)
#define SPI_CPOL_BIT        (1 << 29)
#define SPI_CPHA_BIT        (1 << 9)

spi_error_e spi_init(spi_mode_e mode, uint16_t clock_div, spi_perif_e spi_perif) {
    if (spi_perif == SPI_PERIF_VSPI) {
        SPI_BASE = (volatile uint32_t*)0x3FF65000;
    } else if (spi_perif == SPI_PERIF_HSPI) {
        SPI_BASE = (volatile uint32_t*)0x3FF64000;
    } else {
        return SPI_ERR_NOT_INITIALIZED;
    }

    SPI_CMD_REG = 0;
    SPI_USER_REG = SPI_USR_MOSI | SPI_USR_MISO | SPI_USR_MISO_HIGHPART | SPI_CS_SETUP | SPI_CS_HOLD;

    switch (mode) {
        case SPI_MODE_0:
            SPI_PIN_REG &= ~SPI_CPOL_BIT; // CPOL = 0
            SPI_USER_REG |= SPI_CPHA_BIT;  // CPHA = 0 (CK_OUT_EDGE = 1)
            break;
        case SPI_MODE_1:
            SPI_PIN_REG &= ~SPI_CPOL_BIT; // CPOL = 0
            SPI_USER_REG &= ~SPI_CPHA_BIT; // CPHA = 1 (CK_OUT_EDGE = 0)
            break;
        case SPI_MODE_2:
            SPI_PIN_REG |= SPI_CPOL_BIT;  // CPOL = 1
            SPI_USER_REG |= SPI_CPHA_BIT;  // CPHA = 0 (CK_OUT_EDGE = 1)
            break;
        case SPI_MODE_3:
            SPI_PIN_REG |= SPI_CPOL_BIT;  // CPOL = 1
            SPI_USER_REG &= ~SPI_CPHA_BIT; // CPHA = 1 (CK_OUT_EDGE = 0)
            break;
    }
    
    SPI_CLOCK_REG |= ~(1 << 31);                // Enable pre-divider
    SPI_CLOCK_REG &= ~(0x7FF << 18);           // Clear existing divider
    SPI_CLOCK_REG |= ((clock_div & 0x7FF) << 18); // Set new divider
    
    SPI_PIN_REG |= (1 << 29); // Keep CS high when idle

    spi_initialized = true;
    return SPI_OK;
}

spi_error_e spi_buffer_data(uint32_t data) {
    if (!spi_initialized) return SPI_ERR_NOT_INITIALIZED;
    if (spi_buffer_count >= MAX_SPI_WORDS) return SPI_ERR_OVERFLOW;
    spi_tx_buffer[spi_buffer_count++] = data;
    return SPI_OK;
}

spi_error_e spi_transaction(void) {
    if (!spi_initialized) return SPI_ERR_NOT_INITIALIZED;

    for (uint8_t i = 0; i < spi_buffer_count; i++) {
        SPI_W_REG(i) = spi_tx_buffer[i];         // TX in W0-W7
    }

    SPI_CMD_REG |= SPI_USR;
    while (SPI_CMD_REG & SPI_USR);

    for (uint8_t i = 0; i < spi_buffer_count; i++) {
        spi_rx_buffer[i] = SPI_W_REG(i + 8);     // RX from W8-W15
    }

    spi_buffer_count = 0;
    return SPI_OK;
}

uint32_t spi_received(uint8_t index) {
    if (!spi_initialized || index >= MAX_SPI_WORDS) return 0xFFFFFFFF;
    return spi_rx_buffer[index];
}
