#include "spi.h"
#include "driver/spi_master.h"
#include "spi_config.h"

static spi_device_handle_t spi_handle;

/**
 * @brief Initializes SPI bus and attaches SX1278 device.
 * 
 * @param mode SPI mode (0–3)
 */
void spi_init(uint8_t mode) {
    if (mode >= 4) {return;}

    spi_bus_config_t buscfg = {
        .mosi_io_num = SPI_MOSI,
        .miso_io_num = SPI_MISO,
        .sclk_io_num = SPI_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 1 * 1000 * 1000, // 1 MHz (SX1278 can handle this)
        .mode = 0, // CPOL=0, CPHA=0
        .spics_io_num = SPI_CS,
        .queue_size = 1,
    };

    spi_bus_initialize(SPI_HOST_USED, &buscfg, SPI_DMA_DISABLED);
    spi_bus_add_device(SPI_HOST_USED, &devcfg, &spi_handle);
}

/**
 * @brief Transfers data over SPI.
 * 
 * @param tx_data Transmit buffer (can be NULL).
 * @param rx_data Receive buffer (can be NULL).
 * @param len Number of bytes to transfer.
 * @return esp_err_t result of transmission.
 */
esp_err_t spi_transfer(uint8_t *tx_data, uint8_t *rx_data, size_t len) {
    spi_transaction_t t = {
        .length = len * 8,
        .tx_buffer = tx_data,
        .rx_buffer = rx_data
    };
    return spi_device_transmit(spi_handle, &t);
}

/**
 * @brief Transfers a single byte over SPI.
 * 
 * @param data_out Byte to transmit.
 * @param data_in Pointer to received byte.
 * @return esp_err_t result.
 */
esp_err_t spi_transfer_byte(uint8_t data_out, uint8_t *data_in) {
    return spi_transfer(&data_out, data_in, 1);
}

