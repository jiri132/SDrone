#include "sx1278.h"
#include "sx1278_spi.h"
#include "sx1278_mode.h"
#include "sx1278_config.h"
#include "sx1278_irq.h"
#include "sx1278_fifo.h"
#include "sx1278_defs.h"

static volatile bool tx_active = false;

/**
 * @brief Initializes the SX1278 module.
 * 
 * Sets up pins, SPI, IRQs, and optionally base configuration.
 */
void sx1278_init(void) {
    sx1278_spi_init();                // Setup SPI in Mode 0
    sx1278_reset();                   // Toggle reset pin
    sx1278_set_mode(SX1278_MODE_SLEEP); // Start from a known state
    sx1278_base_config();            // Apply base configuration (SF, BW, etc.)
    sx1278_fifo_init();              // Initialize TX/RX FIFOs
    sx1278_irq_init();               // Enable IRQ callbacks
    sx1278_set_mode(SX1278_MODE_STDBY); // Ready for action
}

/**
 * @brief Sends a packet over LoRa.
 * 
 * @param data Pointer to the buffer to send.
 * @param len Number of bytes to send.
 * @return true if packet was accepted for transmission.
 */
bool sx1278_send(const uint8_t* data, uint8_t len) {
    if (len == 0 || len > 64 || tx_active || !sx1278_is_ready())
        return false;

    sx1278_write_fifo(data, len);    // Write directly into hardware FIFO
    sx1278_write_reg(REG_PAYLOAD_LENGTH, len);
    sx1278_clear_irq_flags();        // Clear stale IRQs
    sx1278_set_mode(SX1278_MODE_TX);
    tx_active = true;
    return true;
}

/**
 * @brief Returns whether the radio is currently transmitting.
 * 
 * @return true if TX is active.
 */
bool sx1278_is_sending(void) {
    return tx_active || sx1278_is_transmitting();
}

/**
 * @brief Handles IRQ-based events such as TxDone or RxDone.
 * 
 * Should be called from `sx1278_irq_dio0_handler()` or loop.
 */
void sx1278_handle_irq(void) {
    uint8_t flags = sx1278_get_irq_flags();

    if (flags & IRQ_TX_DONE_MASK) {
        sx1278_clear_irq_flags();
        tx_active = false;
        sx1278_set_mode(SX1278_MODE_STDBY);
    }

    if (flags & IRQ_RX_DONE_MASK) {
        uint8_t len = sx1278_read_reg(0x13);  // RegRxNbBytes
        uint8_t buf[64];
        if (len <= 64) {
            sx1278_read_fifo(buf, len);
            sx1278_fifo_store_received(buf, len);
        }
        sx1278_clear_irq_flags();
        sx1278_set_mode(SX1278_MODE_RX);  // Resume listening
    }
}

/**
 * @brief Optional polling-based FSM handler.
 * 
 * Call regularly if not interrupt-driven.
 */
void sx1278_tick(void) {
    // polling fallback: restart RX if idle and RX not active
    if (!sx1278_is_sending() && !sx1278_is_receiving()) {
        sx1278_set_mode(SX1278_MODE_RX);
    }
}
