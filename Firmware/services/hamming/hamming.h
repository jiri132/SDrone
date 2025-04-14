#pragma once 

#include "hamming_types.h"

/**
 * @brief Retrieves the configuration parameters for a specified Hamming code type.
 *
 * @param type The Hamming code variant (e.g., HAMMING_7_4, HAMMING_15_11).
 * @return Pointer to a constant HammingConfig struct containing the configuration details,
 *         or NULL if the type is unsupported.
 *
 * @note The returned configuration includes total bits, data bits, parity bits, and SECDED status.
 */
const HammingConfig_t* Hamming_GetConfig(HammingType_e type);

/**
 * @brief Encodes a data word using the selected Hamming configuration.
 *
 * @param data The input data word to encode. Only the least significant k bits are used,
 *             where k is the number of data bits defined by the HammingConfig.
 * @param type The Hamming code type to use for encoding.
 * @return HammingEncodeResult structure containing the encoded word and success status.
 *
 * @note If the input data exceeds the allowable data bit width or the configuration is invalid,
 *       encoding will fail and the success flag will be false.
 */
HammingEncodeResult_t Hamming_Encode(uint32_t data, HammingType_e type);

/**
 * @brief Decodes a Hamming-encoded word and performs single-bit error correction if needed.
 *
 * @param encoded The encoded Hamming word to decode (typically n bits as defined by the configuration).
 * @param type The Hamming code type that was used during encoding.
 * @return HammingDecodeResult structure containing the corrected data, 
 *         whether correction was applied, and if the error was uncorrectable.
 *
 * @note Single-bit errors are corrected automatically. If the syndrome indicates an invalid bit position,
 *       the result will be flagged as uncorrectable.
 */
HammingDecodeResult_t Hamming_Decode(uint32_t encoded, HammingType_e type);

/**
 * @brief Calculates the expected parity bit value for a given parity position.
 *
 * @param word The partially filled Hamming word including data and existing parity bits.
 * @param parity_pos The zero-based index of the parity position (e.g., 0 for p1, 1 for p2, etc.).
 * @return The computed parity bit (0 or 1) for the specified position.
 *
 * @note This function is typically used internally during both encoding and error detection.
 */
uint8_t Hamming_CalculateParity(uint32_t word, uint8_t parity_pos);
