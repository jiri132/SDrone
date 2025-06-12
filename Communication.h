#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

/**
 * @brief Command code definitions combining command ID and data length in nibbles.
 */
#define CMD_ACK        0x0100u
#define CMD_NACK       0x0200u
#define CMD_STATUS     0x0302u
#define CMD_START      0x0400u
#define CMD_STOP       0x0500u
#define CMD_APITCH     0x0602u
#define CMD_AYAW       0x0702u
#define CMD_AROLL      0x0802u
#define CMD_ATHROTTLE  0x0902u
#define CMD_TSPEED     0x0a10u
#define CMD_TAP        0x0b0cu
#define CMD_TRPM       0x0c10u
#define CMD_TCOM       0x0d10u
#define CMD_TERROR     0x0e04u
#define CMD_TPOWER     0x0f08u
#define CMD_TPITCH     0x1008u
#define CMD_TROLL      0x1108u
#define CMD_TYAW       0x1208u
#define CMD_TSOC       0x1304u
#define CMD_TEMF12     0x1410u
#define CMD_TEMF34     0x1510u
#define CMD_TIMOTOR12  0x1610u
#define CMD_TIMOTOR34  0x1710u

/**
 * @brief Enumeration of supported data types in the protocol layer.
 */
enum datatype {
    EMPTY,
    INT8,
    UINT8,
    UINT16,
    FL32,
  	WRONG
};

/**
 * @brief Packed representation of the protocol header.
 */
typedef struct __attribute__((packed))
{
    uint16_t addr;
    uint8_t  cmd;
    uint8_t  len;
} Header;

/**
 * @brief Extracts the data length from a 32-bit protocol header.
 *
 * @param header The full 32-bit header value.
 * @return The number of data bits encoded in the header.
 *
 * @note The length is stored in the lowest byte of the header.
 */
int extractDataLength(uint32_t header);

/**
 * @brief Encodes a command and its arguments into a header and data buffer.
 *
 * @param address   The sending device's address.
 * @param command   The command string (e.g., "STATUS", "ACK").
 * @param argument1 Pointer to the first data argument (optional).
 * @param argument2 Pointer to the second data argument (optional).
 * @param argument3 Pointer to the third data argument (optional).
 * @param argument4 Pointer to the fourth data argument (optional).
 * @param outHeader Pointer to the variable where the 32-bit header will be stored.
 * @param outData   Pointer to the byte array where encoded data will be stored.
 *
 * @note The command string is case-sensitive and must match known identifiers.
 * These can be found in the communication protocol documentation and need to be specifically requested.
 * (Ik lever het wel in in mijn portfolio onder maintainability en algorithms)
 *       Null arguments are ignored.
 */
void protocolEncode(uint16_t address, char command[], void* argument1, void* argument2, void* argument3, void* argument4, uint32_t* outHeader, uint8_t* outData);

/**
 * @brief Decodes a header and data buffer, and prints or processes the command and values.
 *
 * @param header The 32-bit encoded header.
 * @param data   The array of data bytes associated with the command.
 *
 * @note This function handles command-specific logic for interpreting the data payload.
 */
uint16_t protocolDecode(uint32_t header, uint8_t data[]);

#endif