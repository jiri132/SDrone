#pragma once

#define REG_FIFO            0x00
#define REG_OPMODE          0x01
#define REG_FRF_MSB         0x06
#define REG_FRF_MID         0x07
#define REG_FRF_LSB         0x08
#define REG_PA_CONFIG       0x09
#define REG_IRQ_FLAGS       0x12
#define REG_MODEM_CONFIG_1  0x1D
#define REG_MODEM_CONFIG_2  0x1E
#define REG_PAYLOAD_LENGTH  0x22

#define SX1278_LONG_RANGE_MODE    0x80
#define SX1278_SLEEP_MODE         0x00
#define SX1278_STDBY_MODE         0x01
#define SX1278_TX_MODE            0x03
#define SX1278_RX_MODE            0x05

#define IRQ_TX_DONE_MASK          0x08
#define IRQ_RX_DONE_MASK          0x40
#define IRQ_PAYLOAD_CRC_ERROR     0x20
