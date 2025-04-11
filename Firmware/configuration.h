#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// ESP32 Pin connections
#define SX1278_CHIPSELECT 5
#define SX1278_RESET 14
#define SX1278_IRQ 4
#define RX_LED 32
#define TX_LED 33

// SX1278 Settings
#define RADIOFREQUENCY 147.0f // 147MHz
#define BITRATE 125.0f // 125KHz
#define CODERATE 5
#define SPREADFACTOR 7

// Buffer sizes
#define RXBUFFER 0xff
#define TXBUFFER 0Xff

// Message sizes
#define MINIMALLMESSAGE 0x6
#define MAXIMALMESSAGE 0x16

#endif // CONFIGURATION_H
