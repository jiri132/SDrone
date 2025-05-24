#pragma once

#include <stdint.h>
#include <stddef.h>
#include "esp_err.h"

/**
 * @brief Initializes SPI bus and attaches SX1278 device.
 * 
 * @param mode SPI mode (0–3)
 */
void spi_init(uint8_t mode);

/**
 * @brief Transfers data over SPI.
 * 
 * @param tx_data Transmit buffer (can be NULL).
 * @param rx_data Receive buffer (can be NULL).
 * @param len Number of bytes to transfer.
 * @return esp_err_t result of transmission.
 */
esp_err_t spi_transfer(uint8_t *tx_data, uint8_t *rx_data, size_t len);

/**
 * @brief Transfers a single byte over SPI.
 * 
 * @param data_out Byte to transmit.
 * @param data_in Pointer to received byte.
 * @return esp_err_t result.
 */
esp_err_t spi_transfer_byte(uint8_t data_out, uint8_t *data_in);
