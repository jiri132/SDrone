#include "sx1278_mode.h"
#include "sx1278_defs.h"
#include "sx1278_spi.h"
#include "sx1278_pins.h"
#include "sx1278_config.h"
#include "gpio.h"
#include <stdint.h>
#include <stdbool.h>
#include "esp_rom_sys.h"

/**
 * @brief Sets the operating mode of the SX1278 chip.
 * 
 * @param mode Desired mode (e.g., SX1278_MODE_TX).
 * 
 * @note This function handles writing the correct value to the REG_OPMODE register.
 * @note You should always enter standby mode before switching from TX to RX or vice versa.
 */
void sx1278_set_mode(sx1278_mode_t mode) {
    uint8_t current = sx1278_read_reg(REG_OPMODE) & 0xF8;  // mask out mode bits
    sx1278_write_reg(REG_OPMODE, current | (mode & 0x07));
}

/**
 * @brief Reads and returns the current operating mode of the SX1278.
 * 
 * @return sx1278_mode_t The current mode of the radio chip.
 */
sx1278_mode_t sx1278_get_mode(void) {
    uint8_t reg = sx1278_read_reg(REG_OPMODE);
    return (sx1278_mode_t)(reg & 0x07);
}

/**
 * @brief Checks if the SX1278 is in standby mode and ready to accept new commands.
 * 
 * @return true if the device is in standby mode, false otherwise.
 */
bool sx1278_is_ready(void) {
    return sx_get_mode() == SX1278_MODE_STDBY;
}

/**
 * @brief Checks if the SX1278 is currently transmitting.
 * 
 * @return true if in SX1278_MODE_TX, false otherwise.
 */
bool sx1278_is_transmitting(void) {
    return sx_get_mode() == SX1278_MODE_TX;
}

/**
 * @brief Checks if the SX1278 is currently in receive mode.
 * 
 * @return true if in SX1278_MODE_RX, false otherwise.
 */
bool sx1278_is_receiving(void) {
    return sx_get_mode() == SX1278_MODE_RX;
}

/**
 * @brief Performs a hardware reset on the SX1278 using the RESET pin.
 * 
 * This function toggles the RESET pin according to the timing requirements in the datasheet.
 * 
 * @note Requires correct GPIO setup via your HAL.
 */
void sx1278_reset(void) {
    sx1278_reset_low();
    esp_rom_delay_us(100);  // 100 µs (minimale resettijd)
    sx1278_reset_high();
    esp_rom_delay_us(5000); // 5 ms voor chip recovery
}

/**
 * @brief Applies a minimal base configuration to the SX1278.
 * 
 * This function sets default registers for frequency, power, CRC, and other parameters.
 * It is usually called during initialization.
 * 
 * @note For full control, use `sx1278_apply_config()` from `sx1278_config.h`.
 */
void sx1278_base_config(void) {
    // Activate LoRa mode
    uint8_t reg_opmode = sx1278_read_reg(REG_OPMODE);
    sx1278_write_reg(REG_OPMODE, reg_opmode | SX1278_LONG_RANGE_MODE);

    // Set to standby mode
    sx1278_set_mode(SX1278_MODE_STDBY);

    // Default base frequency = 868.1 MHz (EU868)
    sx1278_set_frequency(868100000);

    // Apply basic modem settings
    sx1278_set_spreading_factor(SF7);
    sx1278_set_bandwidth(BW125);
    sx1278_set_coding_rate(CR_4_5);
    sx1278_set_crc(true);

    // Set power level
    sx1278_set_tx_power(14);

    // Payload length = max
    sx1278_write_reg(REG_PAYLOAD_LENGTH, 0xFF);

    // Clear IRQ flags
    sx1278_clear_irq_flags();
}
