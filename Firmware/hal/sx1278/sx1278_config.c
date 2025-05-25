#include "sx1278_config.h" 
#include "sx1278_defs.h"
#include "sx1278_spi.h"

/**
 * @brief Applies a full configuration struct to the SX1278.
 * 
 * @param config Pointer to a filled sx1278_config_t structure.
 * 
 * @note This sets all key parameters: frequency, SF, BW, CR, power, CRC.
 */
void sx1278_apply_config(const sx1278_config_t* config) {
    sx1278_set_frequency(config->frequency_hz);
    sx1278_set_spreading_factor(config->spreading_factor);
    sx1278_set_bandwidth(config->bandwidth);
    sx1278_set_coding_rate(config->coding_rate);
    sx1278_set_crc(config->crc_enabled);
    sx1278_set_tx_power(config->tx_power_dbm);
}

/**
 * @brief Sets the center frequency of the SX1278.
 * 
 * @param freq_hz Frequency in Hz (e.g., 868100000).
 * 
 * @note Frequency must fall within the chip's supported range (137–1020 MHz).
 */
void sx1278_set_frequency(uint64_t freq_hz) {
    uint64_t frf = (uint64_t)((double)freq_hz / 61.03515625); // FXOSC = 32 MHz / 2^19
    sx1278_write_reg(REG_FRF_MSB, (frf >> 16) & 0xFF);
    sx1278_write_reg(REG_FRF_MID, (frf >> 8) & 0xFF);
    sx1278_write_reg(REG_FRF_LSB, frf & 0xFF);
}

/**
 * @brief Sets the spreading factor (SF6–SF12).
 * 
 * @param sf Desired spreading factor.
 * 
 * @note SF6 requires special headerless mode and may not work in all configs.
 */
void sx1278_set_spreading_factor(sx1278_spreading_factor_t sf) {
    uint8_t reg = sx1278_read_reg(REG_MODEM_CONFIG_2);
    reg = (reg & 0x0F) | (sf << 4);
    sx1278_write_reg(REG_MODEM_CONFIG_2, reg);
}

/**
 * @brief Sets the LoRa bandwidth.
 * 
 * @param bw Bandwidth enum (e.g., BW125, BW500).
 */
void sx1278_set_bandwidth(sx1278_bandwidth_t bw) {
    uint8_t reg = sx1278_read_reg(REG_MODEM_CONFIG_1);
    reg = (reg & 0x0F) | (bw << 4);
    sx1278_write_reg(REG_MODEM_CONFIG_1, reg);
}

/**
 * @brief Sets the coding rate (FEC).
 * 
 * @param cr Coding rate enum (e.g., CR_4_5).
 */
void sx1278_set_coding_rate(sx1278_coding_rate_t cr) {
    uint8_t reg = sx1278_read_reg(REG_MODEM_CONFIG_1);
    reg = (reg & 0xF1) | (cr << 1);
    sx1278_write_reg(REG_MODEM_CONFIG_1, reg);
}

/**
 * @brief Enables or disables CRC checking.
 * 
 * @param enabled true to enable CRC, false to disable.
 */
void sx1278_set_crc(bool enabled) {
    uint8_t reg = sx1278_read_reg(REG_MODEM_CONFIG_2);
    if (enabled)
        reg |= (1 << 2);
    else
        reg &= ~(1 << 2);
    sx1278_write_reg(REG_MODEM_CONFIG_2, reg);
}

/**
 * @brief Sets the TX output power.
 * 
 * @param dbm Desired power level in dBm (typically 2–17).
 * 
 * @note Actual limits depend on the selected PA config.
 */
void sx1278_set_tx_power(uint8_t dbm) {
    // Clamp to valid range
    if (dbm > 17) dbm = 17;
    if (dbm < 2) dbm = 2;

    sx1278_write_reg(REG_PA_CONFIG, 0x80 | (dbm - 2)); // PA_BOOST + power level
}

/**
 * @brief Returns a pre-filled default config for EU868 operation.
 * 
 * @return sx1278_config_t struct with typical values:
 *         868.1 MHz, SF7, BW125, CR_4/5, CRC on, 14 dBm
 */
sx1278_config_t sx1278_get_default_config(void) {
    sx1278_config_t cfg = {
        .frequency_hz = 868100000,
        .spreading_factor = SF7,
        .bandwidth = BW125,
        .coding_rate = CR_4_5,
        .crc_enabled = true,
        .tx_power_dbm = 14
    };
    return cfg;
}