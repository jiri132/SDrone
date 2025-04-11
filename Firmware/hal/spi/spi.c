#include "spi.h"
#include <stddef.h>
#include "../gpio/gpio.h"     // Als je je eigen gpio_set_mode etc. nodig hebt

// Eventueel clock gating/reset macros
#define DPORT_BASE 0x3FF00000
#define DPORT_PERIP_RST_EN_REG   (DPORT_BASE + 0x0C)
#define DPORT_PERIP_CLK_EN_REG   (DPORT_BASE + 0x08)
#define DPORT_SPI2_RST           (1 << 18)
#define DPORT_SPI2_CLK_EN        (1 << 18)

// Helpers voor bits in SPI_CMD_REG
#define SPI_CMD_USR  (1 << 24) // Start user-defined transactie

// Helpers voor bits in SPI_USER_REG
#define SPI_USER_MOSI  (1 << 27)
#define SPI_USER_MISO  (1 << 28)
#define SPI_USER_DOUTDIN (1 << 0) // Full-duplex mode (MISO + MOSI)

// In een “echte” HAL zou je meer macros/bits definities toevoegen.

// Kleine hulpfunctie om register te schrijven
static inline void reg_write(volatile uint32_t *addr, uint32_t val) {
    *addr = val;
}

// Kleine hulpfunctie om register te lezen
static inline uint32_t reg_read(volatile uint32_t *addr) {
    return *addr;
}


void spi_init(void)
{
    // 1) Reset SPI2 peripheral
    //    (Zet de RST bit hoog + weer laag)
    volatile uint32_t* rst_reg = (volatile uint32_t*) DPORT_PERIP_RST_EN_REG;
    *rst_reg |=  DPORT_SPI2_RST;
    *rst_reg &= ~DPORT_SPI2_RST;

    // 2) Clock gating SPI2 aanzetten
    volatile uint32_t* clk_reg = (volatile uint32_t*) DPORT_PERIP_CLK_EN_REG;
    *clk_reg |= DPORT_SPI2_CLK_EN;

    // 3) Pin-config (IO MUX) 
    //    Als je bijv. GPIO14 als SCK, GPIO15 MOSI, GPIO12 MISO, GPIO13 CS wilt:
    //    - Configureer IO_MUX registers en stel output enable + input enable in.
    //    - Of roep je eigen gpio_set_mode() enz. aan in OUTPUT/INPUT, 
    //      en routeer via GPIO matrix als je andere pins gebruikt.

    // 4) Master mode config
    //    In SPI_USER_REG en SPI_CTRL_REG stel je CPOL/CPHA, bit orders etc.
    //    Hier even minimalistisch: CPOL=0, CPHA=0, full-duplex
    uint32_t user_val = 0;
    user_val |= SPI_USER_MOSI; 
    user_val |= SPI_USER_MISO; 
    user_val |= SPI_USER_DOUTDIN; // MOSI+MISO tegelijkertijd (full-duplex)
    // Schrijf naar SPI_USER_REG
    reg_write(SPI2_REG(SPI_USER_REG_OFFSET), user_val);

    // 5) Clock config in SPI_CLOCK_REG
    //    Stel b.v. 10 MHz. De TRM beschrijft dat hier 2 delers zijn:
    //    (div_pre + 1) * (div_num + 1). Voor max freq. CPU=80..240.
    //    Even voorbeeld: we doen div_pre=1, div_num=7 => 80MHz/(1+1)/(7+1)=80/2/8=5MHz
    uint32_t clk_val = 0;
    // Bits 0..7: div_num
    // Bits 8..13: div_a
    // Bits 14..19: div_b
    // Bits 20..25: div_pre
    // Bit 31: clk_equ sys_clk
    // Stel hier dus je eigen delers in. 
    // Voor simplicity: div_pre=1, div_num=7, div_a=0, div_b=0 => 5MHz (bij CPU=80MHz)
    clk_val = (1 << 20) | (7); 
    reg_write(SPI2_REG(SPI_CLOCK_REG_OFFSET), clk_val);

    // Overige instellingen (bijv. MSB first, address-cmd phase, etc) 
    // kun je in SPI_CTRL_REG, SPI_USER1/2_REG doen. 
}


void spi_transfer(const uint8_t* data_out, uint8_t* data_in, uint32_t bit_len)
{
    // Voorbeeld: we gaan bit_len bits versturen/ontvangen in full-duplex
    // De SPI_Wx registers (W0..W15) bevatten de data
    //   Elk Wn is 32 bits. Bij bit_len > 32 moet je W1..W2 etc. ook vullen/halen.

    // 1) Schrijf TX-data in W0..Wn. 
    //    Hier even aannemen dat bit_len <= 32, dus 1 woord. 
    //    Als je b.v. 16 bits wilt verzenden, data_out[0..1]. 
    if (data_out) {
        uint32_t val = 0;
        // kopieer b.v. 1..4 bytes in val:
        for (int i = 0; i < (bit_len+7)/8; i++) {
            val |= (data_out[i] << (8*i));
        }
        reg_write(SPI2_REG(SPI_W0_REG_OFFSET), val);
    } else {
        // Geen TX-data => W0 = 0 
        reg_write(SPI2_REG(SPI_W0_REG_OFFSET), 0);
    }

    // 2) Stel MOSI/MISO bit-length in (SPI_MS_DLEN_REG)
    //    MOSI-bitlengte in [17:0], MISO-bitlengte in [31:16] 
    //    Of in nieuwere chips: “usr_mosi_bitlen”, “usr_miso_bitlen” 
    //    In de TRM/ECO vind je of dit 1 register is of aparte reg. 
    //    Een veelgebruikte variant is:
    uint32_t ms_dlen = ((bit_len - 1) << 17) | ((bit_len - 1) << 0);
    reg_write(SPI2_REG(SPI_MS_DLEN_REG_OFFSET), ms_dlen);

    // 3) Start transactie: zet SPI_CMD_REG:USR = 1 
    uint32_t cmd_val = reg_read(SPI2_REG(SPI_CMD_REG_OFFSET));
    cmd_val |= SPI_CMD_USR; 
    reg_write(SPI2_REG(SPI_CMD_REG_OFFSET), cmd_val);

    // 4) Wachten tot hardware klaar is => poll SPI_CMD_REG:USR bit
    while (reg_read(SPI2_REG(SPI_CMD_REG_OFFSET)) & SPI_CMD_USR) {
        // busy-wait
    }

    // 5) Lees RX-data uit W0..Wn (bit_len bits). 
    //    Voor simplicity <= 32 bits:
    if (data_in) {
        uint32_t val = reg_read(SPI2_REG(SPI_W0_REG_OFFSET));
        for (int i=0; i < (bit_len+7)/8; i++) {
            data_in[i] = (val >> (8*i)) & 0xFF;
        }
    }
}

