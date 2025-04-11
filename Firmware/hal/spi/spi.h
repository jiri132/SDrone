#pragma once 
#include <stdint.h>
#include <stdbool.h>

// -- Regio: Defines en constanten --

/**
 * \brief SPI2 (HSPI) base address in DPORT memory (GPIO-subsystem).
 * Voor SPI3 (VSPI) zou dit 0x3FF65000 zijn.
 */
#define DR_REG_SPI2_BASE 0x3FF64000

// Register offsets ten opzichte van DR_REG_SPI2_BASE.
// In de TRM zie je bijv. “0x0” voor CMD_REG, “0x4” voor CTRL_REG, etc. 
#define SPI_CMD_REG_OFFSET          0x000
#define SPI_ADDR_REG_OFFSET         0x004
#define SPI_CTRL_REG_OFFSET         0x008
#define SPI_CLOCK_REG_OFFSET        0x00C
#define SPI_USER_REG_OFFSET         0x010
#define SPI_USER1_REG_OFFSET        0x014
#define SPI_USER2_REG_OFFSET        0x018
#define SPI_MS_DLEN_REG_OFFSET      0x024  // Hier stel je MOSI/MISO bit-length in
#define SPI_W0_REG_OFFSET           0x040  // W0 t/m W15 (0x40 - 0x7C)
  // ...
  // Afhankelijk van wat je nodig hebt, kun je offsets voor meer registers definieren.

// -- Regio: Register access macros --
static inline volatile uint32_t* SPI2_REG(uint32_t offset) {
    return (volatile uint32_t *)(DR_REG_SPI2_BASE + offset);
}

// Voor een complexere setup kun je er ook voor kiezen om #defines te maken:
// #define SPI2_CMD_REG     (*(volatile uint32_t *)(DR_REG_SPI2_BASE + SPI_CMD_REG_OFFSET))
// enzovoort.

// -- Regio: Functie-declaraties --

/**
 * \brief Init SPI2 in master mode, met bepaalde klokinstellingen, CPOL/CPHA, enz.
 */
void spi_init(void);

/**
 * \brief Simpele blocking transfer van N bits (of bytes). 
 * Let op: dit is slechts een basis; je kunt nog DMA, adresfases etc. toevoegen.
 * 
 * \param data_out  pointer naar TX-data (mag NULL zijn als alleen RX)
 * \param data_in   pointer waar RX moet komen (mag NULL zijn als alleen TX)
 * \param bit_len   aantal bits dat verstuurd/ontvangen moet worden
 */
void spi_transfer(const uint8_t* data_out, uint8_t* data_in, uint32_t bit_len);

