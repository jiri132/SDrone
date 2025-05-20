#pragma once

#include <stdint.h>
#include <stddef.h>
#include "esp_err.h"

void spi_init(void);
esp_err_t spi_transfer(uint8_t *tx_data, uint8_t *rx_data, size_t len);
esp_err_t spi_transfer_byte(uint8_t data_out, uint8_t *data_in);
uint8_t sx1278_read_reg(uint8_t reg);