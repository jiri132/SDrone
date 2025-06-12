#pragma once

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Encodes protocol header and data into a Hamming-protected byte stream.
 *
 * This function generates a complete encoded message by taking a protocol address and
 * multiple data fields, packing them into a header and payload, converting the result
 * into 4-bit nibbles, and applying Hamming(8,4) encoding to each nibble.
 *
 * @param[out] outEncodedBytes Pointer to the output buffer that will contain the encoded byte stream.
 *                             The buffer must be large enough to hold the full encoded message.
 * @param[out] outLength Pointer to a variable that will receive the total number of bytes written to outEncodedBytes.
 * @param address The 16-bit destination address to encode into the protocol header.
 * @param command Pointer to the command data (can be NULL if unused).
 * @param d1 Pointer to the first data field (can be NULL if unused).
 * @param d2 Pointer to the second data field (can be NULL if unused).
 * @param d3 Pointer to the third data field (can be NULL if unused).
 * @param d4 Pointer to the fourth data field (can be NULL if unused).
 *
 * @note The encoded output is byte-aligned and suitable for transmission over byte-based interfaces.
 */
void fullEncode(uint8_t* outEncodedBytes, size_t* outLength,
                uint16_t address, void* command, void* d1, void* d2, void* d3, void* d4);

/**
 * @brief Decodes a Hamming-protected byte stream into a protocol header and data fields.
 *
 * This function reverses the process of fullEncode. It takes a stream of encoded bytes,
 * decodes them using Hamming(8,4), merges the resulting 4-bit nibbles back into full bytes,
 * and passes them to the protocol layer for interpretation.
 *
 * @param encodedBytes Pointer to the buffer containing the encoded byte stream.
 *
 * @note This function performs error correction and will log decoding progress and errors.
 *       Decoded content will be passed to protocolDecode() for final interpretation.
 */
uint16_t fullDecode(const uint8_t* encodedBytes);
