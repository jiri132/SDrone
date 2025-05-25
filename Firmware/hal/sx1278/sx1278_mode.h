#pragma once
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Enum representing LoRa operating modes for the SX1278.
 */
typedef enum sx1278_mode {
    SX1278_MODE_SLEEP = 0x00,  /**< Low-power sleep mode */
    SX1278_MODE_STDBY = 0x01,  /**< Standby mode, oscillator on */
    SX1278_MODE_TX    = 0x03,  /**< Transmit mode */
    SX1278_MODE_RX    = 0x05   /**< Receive continuous mode */
} sx1278_mode_t;

/**
 * @brief Sets the operating mode of the SX1278 chip.
 * 
 * @param mode Desired mode (e.g., SX1278_MODE_TX).
 * 
 * @note This function handles writing the correct value to the REG_OPMODE register.
 * @note You should always enter standby mode before switching from TX to RX or vice versa.
 */
void sx1278_set_mode(sx1278_mode_t mode);

/**
 * @brief Reads and returns the current operating mode of the SX1278.
 * 
 * @return sx1278_mode_t The current mode of the radio chip.
 */
sx1278_mode_t sx1278_get_mode(void);

/**
 * @brief Checks if the SX1278 is in standby mode and ready to accept new commands.
 * 
 * @return true if the device is in standby mode, false otherwise.
 */
bool sx1278_is_ready(void);

/**
 * @brief Checks if the SX1278 is currently transmitting.
 * 
 * @return true if in SX1278_MODE_TX, false otherwise.
 */
bool sx1278_is_transmitting(void);

/**
 * @brief Checks if the SX1278 is currently in receive mode.
 * 
 * @return true if in SX1278_MODE_RX, false otherwise.
 */
bool sx1278_is_receiving(void);

/**
 * @brief Performs a hardware reset on the SX1278 using the RESET pin.
 * 
 * This function toggles the RESET pin according to the timing requirements in the datasheet.
 * 
 * @note Requires correct GPIO reset-pin setup.
 */
void sx1278_reset(void);

/**
 * @brief Applies a minimal base configuration to the SX1278.
 * 
 * This function sets default registers for frequency, power, CRC, and other parameters.
 * It is usually called during initialization.
 * 
 * @note For full control, use `sx1278_apply_config()` from `sx1278_config.h`.
 */
void sx1278_base_config(void);
