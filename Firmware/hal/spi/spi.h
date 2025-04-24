#pragma once
#include "spi_types.h"
#include <stdint.h>



/**
 * @brief Initialize the SPI peripheral.
 * @param mode The SPI mode (0-3). For the SX1278, mode 0 is recommended.
 * @param clock_divider The divider value to generate the SPI clock (e.g., divider value that gives ~5-10MHz).
 */
void spi_init(spi_mode_e mode, uint16_t clock_divider);

/**
 * @brief Transfer a 32-bit word over SPI.
 * @param outdata The 32-bit data to send.
 * @return The 32-bit data received from the SPI slave.
 */
uint32_t spi_transfer(uint32_t outdata);
