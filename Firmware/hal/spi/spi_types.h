#pragma once

// SPI mode definitions.
typedef enum spi_mode {
    SPI_MODE_0 = 0, // CPOL = 0, CPHA = 0
    SPI_MODE_1,     // CPOL = 0, CPHA = 1
    SPI_MODE_2,     // CPOL = 1, CPHA = 0
    SPI_MODE_3      // CPOL = 1, CPHA = 1
} spi_mode_e;

typedef enum spi_error {
    SPI_GEN_ERR = -1,
} spi_error_e;

