#include "sx1278_config.h" 

/**
 * @brief Applies a full configuration struct to the SX1278.
 * 
 * @param config Pointer to a filled sx1278_config_t structure.
 * 
 * @note This sets all key parameters: frequency, SF, BW, CR, power, CRC.
 */
void sx1278_apply_config(const sx1278_config_t* config) {
    
}

/**
 * @brief Sets the center frequency of the SX1278.
 * 
 * @param freq_hz Frequency in Hz (e.g., 868100000).
 * 
 * @note Frequency must fall within the chip's supported range (137–1020 MHz).
 */
void sx1278_set_frequency(uint64_t freq_hz) {

}

/**
 * @brief Sets the spreading factor (SF6–SF12).
 * 
 * @param sf Desired spreading factor.
 * 
 * @note SF6 requires special headerless mode and may not work in all configs.
 */
void sx1278_set_spreading_factor(sx1278_spreading_factor_t sf) {

}

/**
 * @brief Sets the LoRa bandwidth.
 * 
 * @param bw Bandwidth enum (e.g., BW125, BW500).
 */
void sx1278_set_bandwidth(sx1278_bandwidth_t bw) {

}

/**
 * @brief Sets the coding rate (FEC).
 * 
 * @param cr Coding rate enum (e.g., CR_4_5).
 */
void sx1278_set_coding_rate(sx1278_coding_rate_t cr) {

}

/**
 * @brief Enables or disables CRC checking.
 * 
 * @param enabled true to enable CRC, false to disable.
 */
void sx1278_set_crc(bool enabled) {

}

/**
 * @brief Sets the TX output power.
 * 
 * @param dbm Desired power level in dBm (typically 2–17).
 * 
 * @note Actual limits depend on the selected PA config.
 */
void sx1278_set_tx_power(uint8_t dbm) {

}

/**
 * @brief Returns a pre-filled default config for EU868 operation.
 * 
 * @return sx1278_config_t struct with typical values:
 *         868.1 MHz, SF7, BW125, CR_4/5, CRC on, 14 dBm
 */
sx1278_config_t sx1278_get_default_config(void) {

}