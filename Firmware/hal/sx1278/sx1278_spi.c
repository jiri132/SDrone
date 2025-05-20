// #include "sx1278_spi.h"
// #include "driver/gpio.h"
// #include "esp_log.h"

// #define TAG "SX1278_SPI"

// #define PIN_NUM_MISO 19
// #define PIN_NUM_MOSI 23
// #define PIN_NUM_CLK  18
// #define PIN_NUM_CS   5

// static spi_device_handle_t sx1278_handle;

// esp_err_t sx1278_spi_init(void) {
//     spi_bus_config_t buscfg = {
//         .mosi_io_num = PIN_NUM_MOSI,
//         .miso_io_num = PIN_NUM_MISO,
//         .sclk_io_num = PIN_NUM_CLK,
//         .quadwp_io_num = -1,
//         .quadhd_io_num = -1,
//         .max_transfer_sz = 0
//     };

//     spi_device_interface_config_t devcfg = {
//         .clock_speed_hz = 1 * 1000 * 1000,  // 1 MHz
//         .mode = 0,                          // SPI mode 0: CPOL=0, CPHA=0
//         .spics_io_num = PIN_NUM_CS,
//         .queue_size = 1,
//     };

//     esp_err_t ret;

//     ret = spi_bus_initialize(VSPI_HOST, &buscfg, SPI_DMA_DISABLED);
//     if (ret != ESP_OK) return ret;

//     ret = spi_bus_add_device(VSPI_HOST, &devcfg, &sx1278_handle);
//     if (ret != ESP_OK) return ret;

//     ESP_LOGI(TAG, "SPI initialized for SX1278");
//     return ESP_OK;
// }

// esp_err_t sx1278_spi_transfer(uint8_t *tx_data, uint8_t *rx_data, size_t length) {
//     spi_transaction_t t = {
//         .length = length * 8,   // bits
//         .tx_buffer = tx_data,
//         .rx_buffer = rx_data,
//     };

//     return spi_device_transmit(sx1278_handle, &t);
// }
