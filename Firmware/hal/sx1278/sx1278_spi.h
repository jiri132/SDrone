#pragma once
#include <stdint.h>

/**
 * @brief Initializes the SPI peripheral for SX1278 communication.
 * 
 * This calls general SPI HAL (spi_init()) with mode 0.
 */
void sx1278_spi_init(void);

/**
 * @brief Writes a single byte to a register.
 * 
 * @param reg Address of the register.
 * @param value Byte value to write.
 * 
 * @note MSB is automatically set to 1 for write operation.
 */
void sx1278_write_reg(uint8_t reg, uint8_t value);

/**
 * @brief Reads a single byte from a register.
 * 
 * @param reg Address of the register.
 * @return Value read from the register.
 * 
 * @note MSB is automatically cleared for read operation.
 */
uint8_t sx1278_read_reg(uint8_t reg);

/**
 * @brief Writes multiple bytes starting at a register address.
 * 
 * @param reg Base register to start writing to.
 * @param data Pointer to the data buffer.
 * @param len Number of bytes to write.
 */
void sx1278_write_burst(uint8_t reg, const uint8_t* data, uint8_t len);

/**
 * @brief Reads multiple bytes starting at a register address.
 * 
 * @param reg Base register to start reading from.
 * @param data Buffer where the received bytes will be stored.
 * @param len Number of bytes to read.
 */
void sx1278_read_burst(uint8_t reg, uint8_t* data, uint8_t len);

/**
 * @brief Reads the IRQ flags register from the SX1278.
 * 
 * @return IRQ flags byte (see datasheet for bit meanings).
 */
uint8_t sx1278_get_irq_flags(void);

/**
 * @brief Clears all IRQ flags by writing to the register.
 * 
 * @note This is typically done after handling a TxDone or RxDone interrupt.
 */
void sx1278_clear_irq_flags(void);
