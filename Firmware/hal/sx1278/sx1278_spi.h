#pragma once

#include <stdint.h>
#include <stdbool.h>

// Als je eigen GPIO definities hier nodig zijn:
#include "../gpio/gpio_config.h"

// Base address en offsets voor SPI2 (HSPI)
#define DR_REG_SPI2_BASE         0x3FF64000

#define SPI_CMD_REG_OFFSET       0x000
#define SPI_ADDR_REG_OFFSET      0x004
#define SPI_CTRL_REG_OFFSET      0x008
#define SPI_CLOCK_REG_OFFSET     0x00C
#define SPI_USER_REG_OFFSET      0x010
#define SPI_USER1_REG_OFFSET     0x014
#define SPI_USER2_REG_OFFSET     0x018
#define SPI_MS_DLEN_REG_OFFSET   0x024
#define SPI_W0_REG_OFFSET        0x040
// ... als je meer nodig hebt, kun je nog W1...W15 en andere defines bijvoegen.

// Externe functies
void spi_sx1278_init(void);

/**
 * @brief Schrijf één register in de SX1278 (address + 1 byte data).
 *
 * @param address  het SX1278‐registeradres (7 bits)
 * @param value    de te schrijven databyte
 */
void sx1278_write_reg(uint8_t address, uint8_t value);

/**
 * @brief Lees één register uit de SX1278 (address + 1 byte data).
 *
 * @param address  het SX1278‐registeradres (7 bits)
 * @return uitgelezen waarde
 */
uint8_t sx1278_read_reg(uint8_t address);
