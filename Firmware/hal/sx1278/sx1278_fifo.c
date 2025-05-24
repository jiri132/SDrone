#include "sx1278_fifo.h" 


/**
 * @brief Initializes the internal TX and optional RX FIFO buffer.
 * 
 * This function should be called once during SX1278 setup.
 */
void sx1278_fifo_init(void) {

}

/**
 * @brief Queues a packet for transmission via the SX1278.
 * 
 * @param data Pointer to the data buffer to enqueue.
 * @param len Number of bytes to enqueue (typically <= 64).
 * @return true if successfully queued, false if not enough space.
 */
bool sx1278_fifo_enqueue(const uint8_t* data, uint8_t len) {

}

/**
 * @brief Retrieves the next packet to be sent from the transmit queue.
 * 
 * @param out_data Buffer where the packet will be written.
 * @param out_len Pointer to a uint8_t where the packet length will be stored.
 * @return true if a packet was available, false if the queue was empty.
 */
bool sx1278_fifo_dequeue(uint8_t* out_data, uint8_t* out_len) {

}

/**
 * @brief Checks whether there is at least one packet in the TX buffer.
 * 
 * @return true if TX buffer is not empty, false otherwise.
 */
bool sx1278_fifo_has_data(void) {

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

}

/**
 * @brief Stores an incoming RX payload in a circular RX buffer.
 * 
 * @param data Pointer to received data.
 * @param len Length of the received payload.
 * @return true if successfully stored, false on overflow.
 */
bool sx1278_fifo_store_received(const uint8_t* data, uint8_t len) {

}

/**
 * @brief Reads a packet from the RX queue.
 * 
 * @param out_data Buffer where the packet will be written.
 * @param out_len Pointer to where the length will be stored.
 * @return true if a packet was available, false if empty.
 */
bool sx1278_fifo_read_received(uint8_t* out_data, uint8_t* out_len) {

}

/**
 * @brief Checks whether the RX buffer has at least one stored payload.
 * 
 * @return true if RX buffer is not empty, false otherwise.
 */
bool sx1278_fifo_has_received(void) {
    
}
