#pragma once

#include <stdint.h>
#include <stdbool.h>

#define BIT_SET(val,bit) ((val) | (1UL << (bit)))
#define BIT_CLEAR(val,bit) ((val) & ~(1UL << (bit)))
#define BIT_TOGGLE(val,bit) ((val) ^ (1UL << (bit)))
#define BIT_CHECK(val,bit) ((val) >> ((bit) & (1U)))

typedef enum HammingType {
    HAMMING_74 = 0,    // Original hamming code
    HAMMING_84 = 1,    // extra bit for double bit error detection
    HAMMING_1511 = 2,  // slightly larger hamming for more data
    HAMMING_1611 = 3,  // Extra parity for double bit error detection
    HAMMING_3126 = 4,  // largest hamming possible
    HAMMING_3226 = 5,  // Largest hamming with double bit error detection 
} HammingType_e;

typedef struct HammingConfig {
    HammingType_e type;
    uint8_t total_bits;
    uint8_t data_bits;
    uint8_t parity_bits;
    bool has_overall_parity;
} HammingConfig_t;

typedef struct HammingEncodeResult {
    uint32_t encoded;
    bool success;
} HammingEncodeResult_t;

typedef struct HammingDecodeResult {
    uint32_t decoded;
    bool corrected;
    bool uncorrectable;
    uint8_t error_position;
} HammingDecodeResult_t;