#include "Communication.h"
#include "hamming.h"
#include "hamming_types.h"
#include "protocolLayer.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define ADDRESS 0x1234u
#define COMMAND "STATUS"
#define DATA_1 0x7Fu
#define DATA_2 NULL
#define DATA_3 NULL
#define DATA_4 NULL

int FLIP_POS = 108;

int main(void) {
    uint8_t encoded[1024];
    uint8_t decoded[1024];
    size_t encodedLength = 0;
    uint8_t testEncoded[12] = {0x00, 0x99, 0x00, 0x1E, 0x1E, 0xAA, 0x87, 0x99, 0xB4, 0xFF, 0x00, 0x00};

    //fullEncode(encoded, &encodedLength, ADDRESS, COMMAND, DATA_1, DATA_2, DATA_3, DATA_4);

    //printf("Encoded Message (%zu bytes):\n", encodedLength);
    //for (size_t i = 0; i < encodedLength; ++i) {
    //    printf("%02X ", encoded[i]);
    //}
    //printf("\n");

    uint16_t a = fullDecode(testEncoded);
	printf("Command: %04X\n", a);
    return 0;
}
