#pragma once

#define SPI_BASE 0x3FF43000


#define SPI_CMD_REG (*(volatile uint32_t*) (SPI_BASE + 0x00))
#define SPI_ADDR_RED (*(volatile uint32_t*) (SPI_BASE + 0x04))
#define SPI_CTRL_REG (*(volatile uint32_t*) (SPI_BASE + 0x08))
#define SPI_CTRL1_REG (*(volatile uint32_t*) (SPI_BASE + 0x0C))
#define SPI_RD_STATUS_REG (*(volatile uint32_t*) (SPI_BASE + 0x10))
#define SPI_CTRL2_REG (*(volatile uint32_t*) (SPI_BASE + 0x14))
#define SPI_CLOCK_REG (*(volatile uint32_t*) (SPI_BASE + 0x18))
#define SPI_USER_REG (*(volatile uint32_t*) (SPI_BASE + 0x1C))
//#define SPI_USER1_REG (*(volatile uint32_t*) (SPI_BASE + 0x20)) (half-duplex)
//#define SPI_USER2_REG (*(volatile uint32_t*) (SPI_BASE + 0x24)) (half-duplex)
#define SPI_MOSI_DLEN_REG (*(volatile uint32_t*) (SPI_BASE + 0x28))
#define SPI_MISO_DLEN_REG (*(volatile uint32_t*) (SPI_BASE + 0x2C))
#define SPI_SLV_WR_STATUS_REG (*(volatile uint32_t*) (SPI_BASE + 0x30))
#define SPI_PIN_REG (*(volatile uint32_t*) (SPI_BASE + 0x34))
#define SPI_SLAVE_REG (*(volatile uint32_t*) (SPI_BASE + 0x38))
#define SPI_SLAVE1_REG (*(volatile uint32_t*) (SPI_BASE + 0x3C))
#define SPI_SLAVE2_REG (*(volatile uint32_t*) (SPI_BASE + 0x40))
#define SPI_SLAVE3_REG (*(volatile uint32_t*) (SPI_BASE + 0x44))
#define SPI_SLV_WRBUF_DLEN_REG (*(volatile uint32_t*) (SPI_BASE + 0x48))
#define SPI_SLV_RDBUF_REG (*(volatile uint32_t*) (SPI_BASE + 0x4C))
#define SPI_SLV_RD_BIT_REG (*(volatile uint32_t*) (SPI_BASE + 0x64))
#define SPI_Wn_REG(n) (*(volatile uint32_t*) (SPI_BASE + 0x80 + (4*(n))))
#define SPI_TX_CRC_REG (*(volatile uint32_t*) (SPI_BASE + 0xC0))
#define SPI_EXT2_REG (*(volatile uint32_t*) (SPI_BASE + 0xF8))

