#include "spi_registers.h"
#include "spi.h"
#include "../gpio/gpio.h"

// Bit definitions (example values):
#define SPI_USR_BIT             (1 << 18)  // When set in SPI_CMD_REG, starts a user-defined SPI transaction.
#define SPI_USR_MOSI_BIT        (1 << 27)  // Enable MOSI phase. (half-duplex mode)
#define SPI_USR_MISO_BIT        (1 << 28)  // Enable MISO phase. (half-duplex mode)
#define SPI_USR_MOSI_HIGHPART        (1 << 25)  // MOSI data will be stored in reg W8~W15
#define SPI_USR_MISO_HIGHPART        (1 << 24)  // MISO data will be stored in reg W8~W15


// For this example, we assume in SPI_USER_REG that:
// Bit 0: CPHA, Bit 1: CPOL. (Check your TRM for the correct bit positions.)
#define SPI_CPHA_BIT            (1 << 0)
#define SPI_CPOL_BIT            (1 << 1)


static const uint32_t get_mosi_data(uint8_t address) {
    return SPI_Wn_REG(address + 8);
}

/**
 * @brief Initialize the SPI peripheral.
 *
 * This function sets the SPI mode and clock divider. It assumes that the VSPI peripheral is chosen.
 */
void spi_init(spi_mode_e mode, uint16_t clock_divider)
{
    // Reset or clear command register.
    SPI_CMD_REG = 0;

    // Clear the SPI_USER_REG before setting options.
    SPI_USER_REG = 0;



    // Configure the SPI mode (CPOL and CPHA).
    switch(mode) {
        case SPI_MODE_0:
            // CPOL = 0, CPHA = 0.
            SPI_USER_REG &= ~((uint32_t)SPI_CPHA_BIT | SPI_CPOL_BIT);
            break;
        case SPI_MODE_1:
            // CPOL = 0, CPHA = 1.
            SPI_USER_REG |= SPI_CPHA_BIT;
            SPI_USER_REG &= ~SPI_CPOL_BIT;
            break;
        case SPI_MODE_2:
            // CPOL = 1, CPHA = 0.
            SPI_USER_REG |= SPI_CPOL_BIT;
            SPI_USER_REG &= ~SPI_CPHA_BIT;
            break;
        case SPI_MODE_3:
            // CPOL = 1, CPHA = 1.
            SPI_USER_REG |= (SPI_CPHA_BIT | SPI_CPOL_BIT);
            break;
        default:
            // Default to mode 0.
            SPI_USER_REG &= ~((uint32_t)SPI_CPHA_BIT | SPI_CPOL_BIT);
            break;
    }

    // Configure the clock divider.
    // In this simplified example, we assume SPI_CLOCK_REG uses the lower 16 bits for the divider.
    // Actual register layout may be more complex (integer/fractional parts).
    SPI_CLOCK_REG |= (1 << 31);
    SPI_CLOCK_REG &= ~(0x7FF << 18);
    SPI_CLOCK_REG |= (clock_divider << 18);
    
    // Set Slave to master data in register 8 to 15
    SPI_USER_REG |= SPI_USR_MISO_HIGHPART;

    // Keep Chip-Select high when idle
    SPI_PIN_REG |= (1 << 30);
}

/**
 * @brief Transfer a 32-bit word over SPI.
 *
 * This function writes a 32-bit word to the transmit register, starts an SPI transaction,
 * waits for its completion, and then reads the received 32-bit word.
 */
uint32_t spi_transfer(uint32_t outdata)
{
    // gpio_write(16, 1); // Turn LED ON

    SPI_Wn_REG(0) = outdata;
    SPI_CMD_REG |= SPI_USR_BIT;
    while (SPI_CMD_REG & SPI_USR_BIT); // Wait for transaction complete

    // gpio_write(16, 0); // Turn LED OFF

    return get_mosi_data(0);
}

