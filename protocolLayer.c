#include "Communication.h"
#include "hamming.h"
#include "hamming_types.h"
#include "protocolLayer.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MAX_DATA_LEN 16
#define MAX_NIBBLES (2 * (sizeof(uint32_t) + MAX_DATA_LEN))

static void snip(const void* data, size_t size, uint8_t* out) {
    const uint8_t* bytes = (const uint8_t*)data;
    for (size_t i = 0; i < size; ++i) {
        out[i * 2]     = (bytes[i] >> 4) & 0x0F;
        out[i * 2 + 1] = bytes[i] & 0x0F;
    }
}

static void mergeNibbles(const uint8_t* nibbles, size_t count, uint8_t* out) {
    for (size_t i = 0; i < count / 2; ++i) {
        out[i] = (nibbles[i * 2] << 4) | (nibbles[i * 2 + 1] & 0x0F);
    }
}

static void encodeNibbles(const uint8_t* inputNibbles, uint32_t* outputEncoded, size_t nibbleCount, HammingType_e type) {
    for (size_t i = 0; i < nibbleCount; ++i) {
        HammingEncodeResult_t enc = Hamming_Encode(inputNibbles[i], type);
        outputEncoded[i] = enc.success ? enc.encoded : 0xFFFFFFFF;
    }
}

static void decodeNibbles(const uint32_t* inputEncoded, uint8_t* outputNibbles, size_t nibbleCount, HammingType_e type) {
    for (size_t i = 0; i < nibbleCount; ++i) {
        HammingDecodeResult_t dec = Hamming_Decode(inputEncoded[i], type);
        outputNibbles[i] = (!dec.uncorrectable) ? (dec.decoded & 0x0F) : 0xFF;
    }
}

void fullEncode(uint8_t* outEncodedBytes, size_t* outLength,
                uint16_t address, void* command, void* d1, void* d2, void* d3, void* d4) {
    uint32_t header = 0;
    uint8_t data[MAX_DATA_LEN] = {0};

    protocolEncode(address, command, d1, d2, d3, d4, &header, data);
    int dataLen = extractDataLength(header);
    size_t totalBytes = sizeof(uint32_t) + dataLen;
    size_t totalNibbles = totalBytes * 2;

    uint8_t nibbles[MAX_NIBBLES] = {0};
    snip(&header, sizeof(uint32_t), nibbles);
    snip(data, dataLen, nibbles + sizeof(uint32_t) * 2);

    uint32_t encoded[MAX_NIBBLES] = {0};
    encodeNibbles(nibbles, encoded, totalNibbles, HAMMING_84);

    for (size_t i = 0; i < totalNibbles; ++i) {
        outEncodedBytes[i] = encoded[i] & 0xFF;
    }

    *outLength = totalNibbles;
}


uint16_t fullDecode(const uint8_t* encodedBytes) {
    const size_t headerNibblesCount = sizeof(uint32_t) * 2;
    uint32_t headerEncoded[headerNibblesCount];
    for (size_t i = 0; i < headerNibblesCount; ++i) {
        headerEncoded[i] = (uint32_t)encodedBytes[i];
    }

    uint8_t decodedHeaderNibbles[headerNibblesCount];
    decodeNibbles(headerEncoded, decodedHeaderNibbles, headerNibblesCount, HAMMING_84);

    uint8_t headerBytes[sizeof(uint32_t)];
    mergeNibbles(decodedHeaderNibbles, headerNibblesCount, headerBytes);

    uint32_t header = 0;
    memcpy(&header, headerBytes, sizeof(uint32_t));

    int dataLen = extractDataLength(header);
    size_t totalBytes = sizeof(uint32_t) + dataLen;
    size_t totalNibbles = totalBytes * 2;

    uint32_t encoded[MAX_NIBBLES];
    for (size_t i = 0; i < totalNibbles; ++i) {
        encoded[i] = (uint32_t)encodedBytes[i];
    }

    uint8_t decodedNibbles[MAX_NIBBLES];
    decodeNibbles(encoded, decodedNibbles, totalNibbles, HAMMING_84);

    uint8_t reconstructed[MAX_DATA_LEN + sizeof(uint32_t)] = {0};
    mergeNibbles(decodedNibbles, totalNibbles, reconstructed);

    uint8_t* data = reconstructed + sizeof(uint32_t);

    printf("Reconstructed Bytes:");
    for (size_t i = 0; i < totalBytes; ++i) {
        printf(" %02X", reconstructed[i]);
    }
    printf("\n");

    uint16_t outputCmd = protocolDecode(header, data);
    return outputCmd;
}
