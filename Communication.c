#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "Communication.h"

//-----Encode-----

enum datatype lookupDatatype(char command[]) {
    if (strcmp(command, "ACK") == 0)            { return EMPTY; }
    else if (strcmp(command, "NACK") == 0)      { return EMPTY; }
    else if (strcmp(command, "STATUS") == 0)    { return UINT8; }
    else if (strcmp(command, "START") == 0)     { return EMPTY; }
    else if (strcmp(command, "STOP") == 0)      { return EMPTY; }
    else if (strcmp(command, "APITCH") == 0)    { return INT8; }
    else if (strcmp(command, "AYAW") == 0)      { return INT8; }
    else if (strcmp(command, "AROLL") == 0)     { return INT8; }
    else if (strcmp(command, "ATHROTTLE") == 0) { return INT8; }
    else if (strcmp(command, "TSPEED") == 0)    { return FL32; }
    else if (strcmp(command, "TAP") == 0)       { return FL32; }
    else if (strcmp(command, "TRPM") == 0)      { return UINT16; }
    else if (strcmp(command, "TCOM") == 0)      { return UINT16; }
    else if (strcmp(command, "TERROR") == 0)    { return UINT16; }
    else if (strcmp(command, "TPOWER") == 0)    { return UINT16; }
    else if (strcmp(command, "TPITCH") == 0)    { return FL32; }
    else if (strcmp(command, "TROLL") == 0)     { return FL32; }
    else if (strcmp(command, "TYAW") == 0)      { return FL32; }
    else if (strcmp(command, "TSOC") == 0)      { return FL32; }
    else if (strcmp(command, "TEMF12") == 0)    { return FL32; }
    else if (strcmp(command, "TEMF34") == 0)    { return FL32; }
    else if (strcmp(command, "TIMOTOR12") == 0) { return FL32; }
    else if (strcmp(command, "TIMOTOR34") == 0) { return FL32; }
    else                                        { return WRONG; }
}

int lookupDataTypeLength(char command[]) {
    enum datatype Datatype = lookupDatatype(command);
    if (Datatype == EMPTY)     { return 0; }
    else if (Datatype == INT8) { return 8; }
    else if (Datatype == UINT8){ return 8; }
    else if (Datatype == UINT16){ return 16; }
    else if (Datatype == FL32) { return 32; }
    else                       { return -1; }
}

int extractDataLength(uint32_t header) {
    return (int)(header & 0x000000FF);
}

uint32_t toBinaryHeader(char command[], uint16_t address) {
    uint32_t binaryRep = 0x00000000;
    uint32_t tempBytes = 0x00000000;

    binaryRep = ((address & 0x0000FFFF) << 16);

    if (strcmp(command, "ACK") == 0)              { tempBytes = CMD_ACK;         }
    else if (strcmp(command, "NACK") == 0)        { tempBytes = CMD_NACK;        }
    else if (strcmp(command, "STATUS") == 0)      { tempBytes = CMD_STATUS;      }
    else if (strcmp(command, "START") == 0)       { tempBytes = CMD_START;       }
    else if (strcmp(command, "STOP") == 0)        { tempBytes = CMD_STOP;        }
    else if (strcmp(command, "APITCH") == 0)      { tempBytes = CMD_APITCH;      }
    else if (strcmp(command, "AYAW") == 0)        { tempBytes = CMD_AYAW;        }
    else if (strcmp(command, "AROLL") == 0)       { tempBytes = CMD_AROLL;       }
    else if (strcmp(command, "ATHROTTLE") == 0)   { tempBytes = CMD_ATHROTTLE;   }
    else if (strcmp(command, "TSPEED") == 0)      { tempBytes = CMD_TSPEED;      }
    else if (strcmp(command, "TAP") == 0)         { tempBytes = CMD_TAP;         }
    else if (strcmp(command, "TRPM") == 0)        { tempBytes = CMD_TRPM;        }
    else if (strcmp(command, "TCOM") == 0)        { tempBytes = CMD_TCOM;        }
    else if (strcmp(command, "TERROR") == 0)      { tempBytes = CMD_TERROR;      }
    else if (strcmp(command, "TPOWER") == 0)      { tempBytes = CMD_TPOWER;      }
    else if (strcmp(command, "TPITCH") == 0)      { tempBytes = CMD_TPITCH;      }
    else if (strcmp(command, "TROLL") == 0)       { tempBytes = CMD_TROLL;       }
    else if (strcmp(command, "TYAW") == 0)        { tempBytes = CMD_TYAW;        }
    else if (strcmp(command, "TSOC") == 0)        { tempBytes = CMD_TSOC;        }
    else if (strcmp(command, "TEMF12") == 0)      { tempBytes = CMD_TEMF12;      }
    else if (strcmp(command, "TEMF34") == 0)      { tempBytes = CMD_TEMF34;      }
    else if (strcmp(command, "TIMOTOR12") == 0)   { tempBytes = CMD_TIMOTOR12;   }
    else if (strcmp(command, "TIMOTOR34") == 0)   { tempBytes = CMD_TIMOTOR34;   }

    binaryRep |= tempBytes;
    return binaryRep;
}

uint64_t toBinary(char command[], void* argument1, void* argument2, void* argument3, void* argument4) {
    uint64_t binaryRep = 0x0000000000000000ULL;
    uint64_t tempBytes = 0x0000000000000000ULL;

    int shiftAmount = lookupDataTypeLength(command);
    if (shiftAmount <= 0) return binaryRep;

    if (argument1 != NULL && (1 * shiftAmount) <= 64) {
        tempBytes = 0x0000000000000000ULL;
        tempBytes |= (uint64_t)(uintptr_t)argument1;
        tempBytes <<= (64 - 1 * shiftAmount);
        binaryRep |= tempBytes;
    }
    if (argument2 != NULL && (2 * shiftAmount) <= 64) {
        tempBytes = 0x0000000000000000ULL;
        tempBytes |= (uint64_t)(uintptr_t)argument2;
        tempBytes <<= (64 - 2 * shiftAmount);
        binaryRep |= tempBytes;
    }
    if (argument3 != NULL && (3 * shiftAmount) <= 64) {
        tempBytes = 0x0000000000000000ULL;
        tempBytes |= (uint64_t)(uintptr_t)argument3;
        tempBytes <<= (64 - 3 * shiftAmount);
        binaryRep |= tempBytes;
    }
    if (argument4 != NULL && (4 * shiftAmount) <= 64) {
        tempBytes = 0x0000000000000000ULL;
        tempBytes |= (uint64_t)(uintptr_t)argument4;
        tempBytes <<= (64 - 4 * shiftAmount);
        binaryRep |= tempBytes;
    }

    return binaryRep;
}

void protocolEncode(uint16_t address, char command[], void* argument1, void* argument2, void* argument3, void* argument4,
            uint32_t* outHeader, uint8_t* outData) {
    uint32_t binaryHeader = toBinaryHeader(command, address);
    uint64_t binaryRep = toBinary(command, argument1, argument2, argument3, argument4);
    printf("Header: %08X\n", binaryHeader);

    int dataSize = extractDataLength(binaryHeader) / 2;
    *outHeader = binaryHeader;

    for (int i = 0; i < dataSize; i++) {
        outData[i] = (uint8_t)(binaryRep >> (64 - 8 * (i + 1)));
    }

    for (int i = 0; i < dataSize; i++) {
        printf("Data bytes: %02X\n", outData[i]);
    }
}

//-----Decode-----

const char* commandFromHeader(uint16_t cmd) {
    switch (cmd) {
        case 0x0100: return "ACK";
        case 0x0200: return "NACK";
        case 0x0302: return "STATUS";
        case 0x0400: return "START";
        case 0x0500: return "STOP";
        case 0x0602: return "APITCH";
        case 0x0702: return "AYAW";
        case 0x0802: return "AROLL";
        case 0x0902: return "ATHROTTLE";
        case 0x0a10: return "TSPEED";
        case 0x0b0c: return "TAP";
        case 0x0c10: return "TRPM";
        case 0x0d10: return "TCOM";
        case 0x0e04: return "TERROR";
        case 0x0f08: return "TPOWER";
        case 0x1008: return "TPITCH";
        case 0x1108: return "TROLL";
		case 0x1208: return "TYAW";
		case 0x1308: return "TSOC";
		case 0x1408: return "TEMF12";
		case 0x1508: return "TEMF34";
		case 0x1608: return "TIMOTOR12";
		case 0x1708: return "TIMOTOR34";
		default: return "UNKNOWN";
	}
}

uint16_t protocolDecode(uint32_t header, uint8_t data[]) {
    int dataSize = extractDataLength(header);
    uint16_t address = header >> 16;
    uint16_t commandCode = header & 0xFFFF;

    const char* commandStr = commandFromHeader(commandCode);
    int dataLengthBits = lookupDataTypeLength((char*)commandStr);
    int argSize = dataLengthBits / 8;

    printf("Address: %04X\n", address);
    printf("Command: \"%s\"\n", commandStr);
    printf("DataLength: %d bytes\n", dataSize);

    if (argSize == 0 || dataSize == 0) return;

    int totalArgs = dataSize / (2 * argSize);

    for (int i = 0; i < totalArgs; i++) {
        uint64_t val = 0;
        for (int j = 0; j < argSize; j++) {
            val <<= 8;
            val |= data[i * argSize + j];
        }
        printf("Argument %d: 0x%0*lX\n", i + 1, argSize * 2, val);
    }
    return commandCode;
}

