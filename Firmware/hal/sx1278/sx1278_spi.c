#include "sx1278_spi.h"
#include "../gpio/gpio.h"  // jouw GPIO HAL
#include <stddef.h>

// DPORT-reg voor reset/clock gating (optioneel)
#define DPORT_BASE               0x3FF00000
#define DPORT_PERIP_RST_EN_REG   (DPORT_BASE + 0x0C)
#define DPORT_PERIP_CLK_EN_REG   (DPORT_BASE + 0x08)
#define DPORT_SPI2_RST           (1 << 18)
#define DPORT_SPI2_CLK_EN        (1 << 18)

static inline volatile uint32_t* spi2_reg(uint32_t offset) {
    return (volatile uint32_t *)(DR_REG_SPI2_BASE + offset);
}

// Helper macro’s om register te schrijven/lezen
static inline void reg_write(volatile uint32_t *addr, uint32_t val) { *addr = val; }
static inline uint32_t reg_read(volatile uint32_t *addr) { return *addr; }

// Bitmask voor CMD‐register (o.a. start “USR”)
#define SPI_CMD_USR        (1 << 24)
// Bits in SPI_USER_REG
#define SPI_USER_MOSI      (1 << 27)
#define SPI_USER_MISO      (1 << 28)
#define SPI_USER_DOUTDIN   (1 << 0)   // full-duplex
// etc.

//---------------------------------------------------------------------------
// Functie: spi_sx1278_init
//---------------------------------------------------------------------------

void spi_sx1278_init(void)
{
    // 1) SPI2 reset & clock gating (optioneel, maar netjes bij init)
    volatile uint32_t* rst_reg = (volatile uint32_t*) DPORT_PERIP_RST_EN_REG;
    *rst_reg |=  DPORT_SPI2_RST;      // in reset
    *rst_reg &= ~DPORT_SPI2_RST;      // reset eraf

    volatile uint32_t* clk_reg = (volatile uint32_t*) DPORT_PERIP_CLK_EN_REG;
    *clk_reg |= DPORT_SPI2_CLK_EN;    // clock aan

    // 2) Configureer NSS-pin (CS) als output, default hoog
    //    (Je gebruikt NSS hier als ‘gewone’ GPIO, zodat je handmatig
    //     kunt toggelen tijdens reads/writes.)
    gpio_set_mode(GPIO_PIN_NSS, GPIO_MODE_OUTPUT);
    gpio_write(GPIO_PIN_NSS, 1);  // chip select in rust = hoog

    // 3) Stel evt. SCK, MOSI, MISO in (als je dat niet al deed)
    //    Voor IO MUX usage, configureer de IOMUX of je eigen gpio_set_mode.
    //    Bijv.:
    //    gpio_set_mode(GPIO14, GPIO_MODE_OUTPUT);  // SCK
    //    gpio_set_mode(GPIO12, GPIO_MODE_INPUT);   // MISO
    //    gpio_set_mode(GPIO15, GPIO_MODE_OUTPUT);  // MOSI
    //    Let wel op: Voor hoge snelheid is direct IOMUX aan te raden.

    // 4) SPI_USER_REG: CPOL=0, CPHA=0 => “mode 0”
    //    Dat is default in registers, maar we stellen het expliciet in:
    uint32_t user_val = 0;
    // MOSI + MISO actief, full-duplex
    user_val |= SPI_USER_MOSI | SPI_USER_MISO | SPI_USER_DOUTDIN;
    reg_write(spi2_reg(SPI_USER_REG_OFFSET), user_val);

    // 5) Clock-instelling => ~8 MHz (bij CPU=80MHz)
    //    De freq = 80MHz / ((div_pre+1)*(div_num+1)).
    //    Voor 80MHz / (1+1)*(5+1) = 80/(2*6)= ~6.66 MHz.
    //    Pas aan naar jouw smaak. Bijv. 80/(2*5)=8 MHz => (div_pre=1, div_num=4).
    uint32_t clk_val = 0;
    // bit0..7: div_num
    // bit20..25: div_pre
    // bit31: clk_equ
    // Voor 8MHz => div_pre=1 en div_num=4
    clk_val = (1 << 20) | (4);
    reg_write(spi2_reg(SPI_CLOCK_REG_OFFSET), clk_val);

    // MOSI/MISO bitorder en byteorder in SPI_CTRL_REG, evt. default laten.
    // Ook kun je hier master/slave config doen. Standaard is master.

    // That's basically het minimum om SPI2 te initialiseren voor SX1278.
}

//---------------------------------------------------------------------------
// Hulpfunctie: simpele SPI-transactie van 1 of 2 bytes
//---------------------------------------------------------------------------
static void spi_transfer_blocking(const void* txbuf, void* rxbuf, int bitlen)
{
    // HIER vereenvoudigd: max 32 bits. SX1278 gebruikt meestal 8 bits per write.
    // 1) Schrijf W0
    uint32_t txv=0;
    if (txbuf) {
        // copieer 1 byte of 2 bytes?
        // We gaan er even van uit 1 byte => bitlen=8. 
        // (Soms 16 bits, om address + data in 1x te versturen, etc.)
        txv = *((uint8_t*)txbuf);
    }
    reg_write(spi2_reg(SPI_W0_REG_OFFSET), txv);

    // 2) Stel MOSI/MISO bit-lengte in via SPI_MS_DLEN_REG
    //    MOSI-bitlen [17:0], MISO-bitlen [31:16]. 
    //    We gebruiken full-duplex, bitlen=8 => 7
    uint32_t ms_dlen = ((bitlen - 1) << 0) | ((bitlen - 1) << 16);
    reg_write(spi2_reg(SPI_MS_DLEN_REG_OFFSET), ms_dlen);

    // 3) CMD_USR zetten
    uint32_t cmd_val = reg_read(spi2_reg(SPI_CMD_REG_OFFSET));
    cmd_val |= SPI_CMD_USR;
    reg_write(spi2_reg(SPI_CMD_REG_OFFSET), cmd_val);

    // 4) Wachten tot klaar
    while (reg_read(spi2_reg(SPI_CMD_REG_OFFSET)) & SPI_CMD_USR) {
        // busy wait
    }

    // 5) Als rxbuf != NULL, lees W0
    if (rxbuf) {
        uint32_t rv = reg_read(spi2_reg(SPI_W0_REG_OFFSET));
        *((uint8_t*)rxbuf) = (uint8_t) (rv & 0xFF);
    }
}

//---------------------------------------------------------------------------
// SX1278: Write register
//---------------------------------------------------------------------------

void sx1278_write_reg(uint8_t address, uint8_t value)
{
    // In SX1278 datasheet:
    //  - MSB=0 => Write, MSB=1 => Read
    //  - Byte 0: 0x00..0x7F => address
    //  - Byte 1: data
    // Dus we sturen 2 bytes: [ address&0x7F, value ]

    // Chip select laag
    gpio_write(GPIO_PIN_NSS, 0);

    // Byte0: adres (bit7=0 => write)
    uint8_t addr = (address & 0x7F);
    spi_transfer_blocking(&addr, NULL, 8);

    // Byte1: data
    spi_transfer_blocking(&value, NULL, 8);

    // CS hoog
    gpio_write(GPIO_PIN_NSS, 1);
}

//---------------------------------------------------------------------------
// SX1278: Read register
//---------------------------------------------------------------------------

uint8_t sx1278_read_reg(uint8_t address)
{
    // Read = MSB=1 => address|0x80
    // We sturen: [ address|0x80 ], dan 1 dummy‐byte om data te ontvangen
    uint8_t addr = address | 0x80;
    uint8_t rx=0;

    // CS laag
    gpio_write(GPIO_PIN_NSS, 0);

    // Stuur address
    spi_transfer_blocking(&addr, NULL, 8);

    // Lees data
    // TX: 0x00, RX: data
    uint8_t dummy_tx = 0;
    spi_transfer_blocking(&dummy_tx, &rx, 8);

    // CS hoog
    gpio_write(GPIO_PIN_NSS, 1);

    return rx;
}
