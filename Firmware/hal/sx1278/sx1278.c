#include "sx1278.h" 

/**
 * @brief Initializes the SX1278 module.
 * 
 * Sets up pins, SPI, IRQs, and optionally base configuration.
 */
void sx1278_init(void) {

}

/**
 * @brief Sends a packet over LoRa.
 * 
 * @param data Pointer to the buffer to send.
 * @param len Number of bytes to send.
 * @return true if packet was accepted for transmission.
 */
bool sx1278_send(const uint8_t* data, uint8_t len) {

}

/**
 * @brief Returns whether the radio is currently transmitting.
 * 
 * @return true if TX is active.
 */
bool sx1278_is_sending(void) {

}

/**
 * @brief Handles IRQ-based events such as TxDone or RxDone.
 * 
 * Should be called from `sx1278_irq_dio0_handler()` or loop.
 */
void sx1278_handle_irq(void) {

}

/**
 * @brief Optional polling-based FSM handler.
 * 
 * Call regularly if not interrupt-driven.
 */
void sx1278_tick(void) {

}
