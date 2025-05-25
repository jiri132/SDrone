#include "sx1278_spi.h"
#include "spi.h"
#include "sx1278_defs.h"   // bevat REG_IRQ_FLAGS enz.

/**
 * @brief Initializes the SPI peripheral for SX1278 communication.
 * 
 * This may call into your general SPI HAL (e.g., spi_init()).
 */
void sx1278_spi_init(void) {
    spi_init(0,20);
}

/**
 * @brief Writes a single byte to a register.
 * 
 * @param reg Address of the register.
 * @param value Byte value to write.
 * 
 * @note MSB is automatically set to 1 for write operation.
 */
void sx1278_write_reg(uint8_t reg, uint8_t value) {
    uint8_t tx[2] = { reg | 0x80, value };  // MSB = 1 → write
    spi_transfer(tx, NULL, 2);
}

/**
 * @brief Reads a single byte from a register.
 * 
 * @param reg Address of the register.
 * @return Value read from the register.
 * 
 * @note MSB is automatically cleared for read operation.
 */
uint8_t sx1278_read_reg(uint8_t reg) {
    uint8_t tx[2] = { reg & 0x7F, 0x00 };   // MSB = 0 → read
    uint8_t rx[2] = { 0 };

    spi_transfer(tx, rx, 2);
    return rx[1];
}

/**
 * @brief Writes multiple bytes starting at a register address.
 * 
 * @param reg Base register to start writing to.
 * @param data Pointer to the data buffer.
 * @param len Number of bytes to write.
 */
void sx1278_write_burst(uint8_t reg, const uint8_t* data, uint8_t len) {
if (len == 0) return;

    uint8_t tx[len + 1];
    tx[0] = reg | 0x80;  // write burst
    for (uint8_t i = 0; i < len; i++) {
        tx[i + 1] = data[i];
    }

    spi_transfer(tx, NULL, len + 1);
}

/**
 * @brief Reads multiple bytes starting at a register address.
 * 
 * @param reg Base register to start reading from.
 * @param data Buffer where the received bytes will be stored.
 * @param len Number of bytes to read.
 */
void sx1278_read_burst(uint8_t reg, uint8_t* data, uint8_t len) {
    if (len == 0) return;

    uint8_t tx[len + 1];
    uint8_t rx[len + 1];
    tx[0] = reg & 0x7F;  // read burst
    for (uint8_t i = 1; i <= len; i++) {
        tx[i] = 0x00;
    }

    spi_transfer(tx, rx, len + 1);

    for (uint8_t i = 0; i < len; i++) {
        data[i] = rx[i + 1];
    }
}

/**
 * @brief Reads the IRQ flags register from the SX1278.
 * 
 * @return IRQ flags byte (see datasheet for bit meanings).
 */
uint8_t sx1278_get_irq_flags(void) {
    return sx1278_read_reg(REG_IRQ_FLAGS);
}

/**
 * @brief Clears all IRQ flags by writing to the register.
 * 
 * @note This is typically done after handling a TxDone or RxDone interrupt.
 */
void sx1278_clear_irq_flags(void) {
    sx1278_write_reg(REG_IRQ_FLAGS, 0xFF);
}
