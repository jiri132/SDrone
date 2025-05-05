#pragma once

#include <stdint.h>
#include <stdbool.h>

// Maximum number of buffered 32-bit SPI words
#define MAX_SPI_WORDS 8

// SPI peripheral options
typedef enum {
    SPI_PERIF_VSPI,
    SPI_PERIF_HSPI
} spi_perif_e;

// SPI mode (CPOL/CPHA)
typedef enum {
    SPI_MODE_0 = 0,
    SPI_MODE_1,
    SPI_MODE_2,
    SPI_MODE_3
} spi_mode_e;

// SPI return status
typedef enum {
    SPI_OK = 0,
    SPI_ERR_OVERFLOW,
    SPI_ERR_NOT_INITIALIZED
} spi_error_e;

/**
 * @brief Initialize the SPI peripheral
 *
 * @param mode        SPI mode (0–3) based on CPOL/CPHA
 * @param clock_div   SPI clock divider (max 0x7FF for slowest)
 * @param spi_perif   SPI peripheral to use (VSPI or HSPI)
 * @return SPI_OK if successful, or error code
 */
spi_error_e spi_init(spi_mode_e mode, uint16_t clock_div, spi_perif_e spi_perif);

/**
 * @brief Buffer a 32-bit value for next SPI transaction
 *
 * @param data  The 32-bit word to buffer
 * @return SPI_OK if buffered, or SPI_ERR_OVERFLOW
 */
spi_error_e spi_buffer_data(uint32_t data);

/**
 * @brief Execute a transaction with the buffered data
 *
 * @return SPI_OK if the transfer completes, or error code
 */
spi_error_e spi_transaction(void);

/**
 * @brief Receive a single 32-bit word from the SPI bus
 *
 * @return The received data word (or 0xFFFFFFFF if not initialized)
 */
uint32_t spi_received(uint8_t index);
