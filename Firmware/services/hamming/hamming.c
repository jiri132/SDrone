#include "hamming.h" 
#include <stdbool.h>

#define TOTAL_CONFIGS 6

static const HammingConfig_t hamming_configs[TOTAL_CONFIGS] = {
    { HAMMING_74, 7,4,3,false},
    { HAMMING_84, 8,4,4,true},
    { HAMMING_1511, 15, 11, 4, false},
    { HAMMING_1611, 16, 11, 5, true},
    { HAMMING_3126, 31, 26, 5, false},
    { HAMMING_3226, 32, 26, 6, true},
};


const HammingConfig_t* hamming_get_config(HammingType_e type) {
    for (int i = 0;  i < (sizeof(hamming_configs) / sizeof(hamming_configs[0])); i++) {
        if (hamming_configs[i].type == type)  {
            return &hamming_configs[i];
        }
    }
    return NULL;
}

uint8_t Hamming_CalculateParity(uint32_t word, uint8_t parity_pos) {
    uint8_t parity = 0;
    for (uint8_t i = 0; i < 32; i++) {
        if (i == parity_pos) continue;
        if ((i + 1) & (1 << parity_pos)) {
            parity ^= (word >> i) & 1;
        }
    }
    return parity;
}

HammingEncodeResult_t Hamming_Encode(uint32_t data, HammingType_e type) {
    const HammingConfig_t *cfg = Hamming_GetConfig(type);
    HammingEncodeResult_t result = { .success = false };

    if (!cfg || cfg->data_bits > 26 || cfg->total_bits > 32) return result;

    uint32_t encoded = 0;
    uint8_t data_bit = 0;
    for (uint8_t i = 0; i < cfg->total_bits; ++i) {
        if ((i & (i + 1)) == 0) continue; // skip parity bits (positions 1, 2, 4, 8,...)
        if (data_bit < cfg->data_bits) {
            encoded |= ((data >> data_bit) & 1) << i;
            data_bit++;
        }
    }

    for (uint8_t i = 0; i < cfg->parity_bits; ++i) {
        uint8_t parity = Hamming_CalculateParity(encoded, i);
        encoded |= (parity << ((1 << i) - 1));
    }

    if (cfg->has_overall_parity) {
        uint8_t parity = __builtin_parity(encoded);
        encoded |= (parity << (cfg->total_bits - 1));
    }

    result.encoded = encoded;
    result.success = true;
    return result;
}

HammingDecodeResult_t Hamming_Decode(uint32_t encoded, HammingType_e type) {
    const HammingConfig_t *cfg = Hamming_GetConfig(type);
    HammingDecodeResult_t result = { .uncorrectable = false };

    if (!cfg) return result;

    uint8_t syndrome = 0;
    for (uint8_t i = 0; i < cfg->parity_bits; ++i) {
        uint8_t parity = Hamming_CalculateParity(encoded, i);
        parity ^= (encoded >> ((1 << i) - 1)) & 1;
        if (parity) syndrome |= (1 << i);
    }

    if (syndrome) {
        if (syndrome <= cfg->total_bits) {
            encoded ^= (1 << (syndrome - 1));
            result.corrected = true;
            result.error_position = syndrome;
        } else {
            result.uncorrectable = true;
            return result;
        }
    }

    uint32_t data = 0;
    uint8_t data_bit = 0;
    for (uint8_t i = 0; i < cfg->total_bits; ++i) {
        if ((i & (i + 1)) == 0) continue; // skip parity bits
        if (data_bit < cfg->data_bits)
            data |= ((encoded >> i) & 1) << data_bit++;
    }

    result.decoded = data;
    return result;
}

