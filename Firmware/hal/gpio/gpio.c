#include "gpio.h"

#define GPIO_BASE 0x3FF44000
#define IO_MUX_BASE 0x3FF49000

#define GPIO_ENABLE_W1TS_REG (*(volatile uint32_t*)(GPIO_BASE + 0x24))
#define GPIO_ENABLE_W1TC_REG (*(volatile uint32_t*)(GPIO_BASE + 0x28))
#define GPIO_OUT_W1TS_REG    (*(volatile uint32_t*)(GPIO_BASE + 0x08))
#define GPIO_OUT_W1TC_REG    (*(volatile uint32_t*)(GPIO_BASE + 0x0C))
#define GPIO_OUT_REG         (*(volatile uint32_t*)(GPIO_BASE + 0x04))
#define GPIO_IN_REG          (*(volatile uint32_t*)(GPIO_BASE + 0x3C))

#define MAX_GPIO_PINS 40

static uint8_t queued_states[MAX_GPIO_PINS];
static bool queued_mask[MAX_GPIO_PINS];

static inline volatile uint32_t* gpio_io_mux(uint8_t pin) {
    return (volatile uint32_t*)(IO_MUX_BASE + pin * 4);
}

void gpio_configure_pin(const gpio_config_t* cfg) {
    if (cfg->pin >= MAX_GPIO_PINS) return;

    // IO_MUX handling (only valid for pins 0–33)
    if (cfg->pin <= 33) {
        volatile uint32_t* mux_reg = gpio_io_mux(cfg->pin);
        *mux_reg &= ~(0b111);         // FUNC bits
        *mux_reg |= 0;                // GPIO func
        *mux_reg &= ~((1 << 6) | (1 << 7)); // Clear pull config

        if (cfg->mode == GPIO_MODE_INPUT_PULLUP) {
            *mux_reg |= (1 << 7); // Pull-up
        } else if (cfg->mode == GPIO_MODE_INPUT_PULLDOWN) {
            *mux_reg |= (1 << 6); // Pull-down
        }
    }

    // Set direction
    if (cfg->mode == GPIO_MODE_OUTPUT) {
        if (cfg->pin < 32) {
            GPIO_ENABLE_W1TS_REG = (1 << cfg->pin);
        } else {
            *(volatile uint32_t*)(GPIO_BASE + 0x30) = (1 << (cfg->pin - 32)); // GPIO_ENABLE1_W1TS_REG
        }
    } else {
        if (cfg->pin < 32) {
            GPIO_ENABLE_W1TC_REG = (1 << cfg->pin);
        } else {
            *(volatile uint32_t*)(GPIO_BASE + 0x34) = (1 << (cfg->pin - 32)); // GPIO_ENABLE1_W1TC_REG
        }
    }
}


void gpio_set_pin(uint8_t pin, uint8_t level) {
    if (pin >= MAX_GPIO_PINS) return;

    if (pin < 32) {
        if (level)
            GPIO_OUT_W1TS_REG = (1 << pin);
        else
            GPIO_OUT_W1TC_REG = (1 << pin);
    } else {
        if (level)
            *(volatile uint32_t*)(GPIO_BASE + 0x14) = (1 << (pin - 32)); // GPIO_OUT1_W1TS
        else
            *(volatile uint32_t*)(GPIO_BASE + 0x18) = (1 << (pin - 32)); // GPIO_OUT1_W1TC
    }
}


void gpio_toggle_pin(uint8_t pin) {
    if (pin >= MAX_GPIO_PINS) return;

    if (pin < 32) {
        GPIO_OUT_REG ^= (1 << pin);
    } else {
        volatile uint32_t* out1 = (volatile uint32_t*)(GPIO_BASE + 0x10); // GPIO_OUT1_REG
        *out1 ^= (1 << (pin - 32));
    }
}


uint8_t gpio_read_pin(uint8_t pin) {
    if (pin >= MAX_GPIO_PINS) return 0;

    if (pin < 32) {
        return (GPIO_IN_REG >> pin) & 0x1;
    } else {
        return (*(volatile uint32_t*)(GPIO_BASE + 0x40) >> (pin - 32)) & 0x1; // GPIO_IN1_REG
    }
}

void gpio_queue_pin_state(uint8_t pin, uint8_t level) {
    if (pin >= MAX_GPIO_PINS) return;
    queued_states[pin] = level;
    queued_mask[pin] = true;
}

void gpio_clear_queued_pin(uint8_t pin) {
    if (pin >= MAX_GPIO_PINS) return;
    queued_mask[pin] = false;
}

void gpio_commit_queue(void) {
    for (uint8_t pin = 0; pin < MAX_GPIO_PINS; ++pin) {
        if (queued_mask[pin]) {
            gpio_set_pin(pin, queued_states[pin]);
            queued_mask[pin] = false;
        }
    }
}

bool gpio_pin_active(uint8_t pin) {
    if (pin >= MAX_GPIO_PINS) return false;
    return gpio_read_pin(pin) == 1;
}

bool gpio_is_output(uint8_t pin) {
    if (pin >= MAX_GPIO_PINS) return false;
    if (pin < 32) {
        volatile uint32_t* enable_reg = (volatile uint32_t*)(0x3FF44020); // GPIO_ENABLE_REG
        return (*enable_reg & (1 << pin)) != 0;
    } else {
        volatile uint32_t* enable1_reg = (volatile uint32_t*)(0x3FF4402C); // GPIO_ENABLE1_REG
        return (*enable1_reg & (1 << (pin - 32))) != 0;
    }
}

bool gpio_is_input(uint8_t pin) {
    return !gpio_is_output(pin);
}
