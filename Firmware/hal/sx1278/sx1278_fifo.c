#include "sx1278_fifo.h" 
#include "sx1278_spi.h"
#include "sx1278_defs.h"
#include "circularbuffer.h"

#define LORA_MAX_PACKET_LEN 64
#define TX_BUFFER_SIZE 128
#define RX_BUFFER_SIZE 128

static CircularBuffer_t tx_queue;
static CircularBuffer_t rx_queue;
static uint8_t tx_storage[TX_BUFFER_SIZE];
static uint8_t rx_storage[RX_BUFFER_SIZE];
/**
 * @brief Initializes the internal TX and optional RX FIFO buffer.
 * 
 * This function should be called once during SX1278 setup.
 */
void sx1278_fifo_init(void) {
    create_buffer(&tx_queue, TX_BUFFER_SIZE);
    tx_queue.data = tx_storage;
    clear_buffer(&tx_queue);

    create_buffer(&rx_queue, RX_BUFFER_SIZE);
    rx_queue.data = rx_storage;
    clear_buffer(&rx_queue);
}

/**
 * @brief Queues a packet for transmission via the SX1278.
 * 
 * @param data Pointer to the data buffer to enqueue.
 * @param len Number of bytes to enqueue (typically <= 64).
 * @return true if successfully queued, false if not enough space.
 */
bool sx1278_fifo_enqueue(const uint8_t* data, uint8_t len) {
    if (len == 0 || len > LORA_MAX_PACKET_LEN) return false;
    if (buffer_space_left(&tx_queue) < len + 1) return false;

    write_buffer(&tx_queue, len, false);
    for (uint8_t i = 0; i < len; i++) {
        write_buffer(&tx_queue, data[i], false);
    }
    return true;
}

/**
 * @brief Retrieves the next packet to be sent from the transmit queue.
 * 
 * @param out_data Buffer where the packet will be written.
 * @param out_len Pointer to a uint8_t where the packet length will be stored.
 * @return true if a packet was available, false if the queue was empty.
 */
bool sx1278_fifo_dequeue(uint8_t* out_data, uint8_t* out_len) {
    if (buffer_is_empty(&tx_queue)) return false;

    *out_len = read_buffer(&tx_queue);
    for (uint8_t i = 0; i < *out_len; i++) {
        out_data[i] = read_buffer(&tx_queue);
    }
    return true;
}

/**
 * @brief Checks whether there is at least one packet in the TX buffer.
 * 
 * @return true if TX buffer is not empty, false otherwise.
 */
bool sx1278_fifo_has_data(void) {
    return !buffer_is_empty(&tx_queue);
}

/**
 * @brief Writes a raw buffer to the SX1278's hardware FIFO.
 * 
 * @param data Pointer to the data buffer.
 * @param len Number of bytes to write (<= 64).
 * 
 * @note This function performs a direct SPI burst write to the FIFO register.
 */
void sx1278_write_fifo(const uint8_t* data, uint8_t len) {
    if (len == 0 || len > LORA_MAX_PACKET_LEN) return;
    sx1278_write_burst(REG_FIFO, data, len);
}

/**
 * @brief Reads raw bytes from the SX1278's FIFO buffer.
 * 
 * @param data Output buffer to store the received data.
 * @param len Number of bytes to read.
 * 
 * @note Used after RX done interrupt to read received payload.
 */
void sx1278_read_fifo(uint8_t* data, uint8_t len) {
    if (len == 0 || len > LORA_MAX_PACKET_LEN) return;
    sx1278_read_burst(REG_FIFO, data, len);
}

/**
 * @brief Stores an incoming RX payload in a circular RX buffer.
 * 
 * @param data Pointer to received data.
 * @param len Length of the received payload.
 * @return true if successfully stored, false on overflow.
 */
bool sx1278_fifo_store_received(const uint8_t* data, uint8_t len) {
    if (len == 0 || len > LORA_MAX_PACKET_LEN) return false;
    if (buffer_space_left(&rx_queue) < len + 1) return false;

    write_buffer(&rx_queue, len, false);
    for (uint8_t i = 0; i < len; i++) {
        write_buffer(&rx_queue, data[i], false);
    }
    return true;
}

/**
 * @brief Reads a packet from the RX queue.
 * 
 * @param out_data Buffer where the packet will be written.
 * @param out_len Pointer to where the length will be stored.
 * @return true if a packet was available, false if empty.
 */
bool sx1278_fifo_read_received(uint8_t* out_data, uint8_t* out_len) {
    if (buffer_is_empty(&rx_queue)) return false;

    *out_len = read_buffer(&rx_queue);
    for (uint8_t i = 0; i < *out_len; i++) {
        out_data[i] = read_buffer(&rx_queue);
    }
    return true;
}

/**
 * @brief Checks whether the RX buffer has at least one stored payload.
 * 
 * @return true if RX buffer is not empty, false otherwise.
 */
bool sx1278_fifo_has_received(void) {
    return !buffer_is_empty(&rx_queue);
}
