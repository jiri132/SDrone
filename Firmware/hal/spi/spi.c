#include "spi.h"
#include "driver/spi_master.h"
#include "spi_config.h"

static spi_device_handle_t spi_handle;

void spi_init(void) {
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

esp_err_t spi_transfer(uint8_t *tx_data, uint8_t *rx_data, size_t len) {
    spi_transaction_t t = {
        .length = len * 8,
        .tx_buffer = tx_data,
        .rx_buffer = rx_data
    };
    return spi_device_transmit(spi_handle, &t);
}

esp_err_t spi_transfer_byte(uint8_t data_out, uint8_t *data_in) {
    return spi_transfer(&data_out, data_in, 1);
}

uint8_t sx1278_read_reg(uint8_t reg) {
    uint8_t tx[2] = { reg & 0x7F, 0x00 }; // Read command
    uint8_t rx[2] = { 0 };

    spi_transfer(tx, rx, 2);          // Send reg addr, receive data

    return rx[1]; // second byte is the register value
}
