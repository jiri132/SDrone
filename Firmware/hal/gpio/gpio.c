#include "gpio.h" 
#include "gpio_config.h"

#define GPIO_BASE               0x3FF44000
#define GPIO_OUT_REG            (*(volatile uint32_t*) (GPIO_BASE + 0x04))
#define GPIO_OUT_W1TS_REG       (*(volatile uint32_t*) (GPIO_BASE + 0x08))// 0X08 
#define GPIO_OUT_W1TC_REG       (*(volatile uint32_t*) (GPIO_BASE + 0x0c))// 0X0C
#define GPIO_OUT1_REG           (*(volatile uint32_t*) (GPIO_BASE + 0x10))// 0X10
#define GPIO_OUT1_W1TS_REG      (*(volatile uint32_t*) (GPIO_BASE + 0x14))// 0X14
#define GPIO_OUT1_W1TC_REG      (*(volatile uint32_t*) (GPIO_BASE + 0x18))// 0X18
#define GPIO_ENABLE_REG         (*(volatile uint32_t*) (GPIO_BASE + 0x20))// 0X20
#define GPIO_ENABLE_W1TS_REG    (*(volatile uint32_t*) (GPIO_BASE + 0x24))// 0X24
#define GPIO_ENABLE_W1TC_REG    (*(volatile uint32_t*) (GPIO_BASE + 0x28))// 0X28
#define GPIO_ENABLE1_REG        (*(volatile uint32_t*) (GPIO_BASE + 0x2c))// 0X2C
#define GPIO_ENABLE1_W1TS_REG   (*(volatile uint32_t*) (GPIO_BASE + 0x30))// 0X30
#define GPIO_ENABLE1_W1TC_REG   (*(volatile uint32_t*) (GPIO_BASE + 0x34))// 0X34
#define GPIO_STRAP_REG          (*(volatile uint32_t*) (GPIO_BASE + 0x38))// 0X38
#define GPIO_IN_REG             (*(volatile uint32_t*) (GPIO_BASE + 0x03c))// 0X3C
#define GPIO_IN1_REG            (*(volatile uint32_t*) (GPIO_BASE + 0x40))// 0X40
#define GPIO_STATUS_REG         (*(volatile uint32_t*) (GPIO_BASE + 0x44))//0X44
#define GPIO_STATUS_W1TS_REG    (*(volatile uint32_t*) (GPIO_BASE + 0x48))//0X48
#define GPIO_STATUS_W1TC_REG    (*(volatile uint32_t*) (GPIO_BASE + 0x4c))//0X4C
#define GPIO_STATUS1_REG        (*(volatile uint32_t*) (GPIO_BASE + 0x50))//0X50
#define GPIO_STATUS1_W1TS_REG   (*(volatile uint32_t*) (GPIO_BASE + 0x54))//0X54
#define GPIO_STATUS1_W1TC_REG   (*(volatile uint32_t*) (GPIO_BASE + 0x58))//0X58
#define GPIO_ACPU_INT_REG       (*(volatile uint32_t*) (GPIO_BASE + 0x60))// 0x60
#define GPIO_ACPU_NMI_INT_REG   (*(volatile uint32_t*) (GPIO_BASE + 0x64))// 0x64
#define GPIO_PCPU_INT_REG       (*(volatile uint32_t*) (GPIO_BASE + 0x68))// 0x68
#define GPIO_PCPU_NMI_INT_REG   (*(volatile uint32_t*) (GPIO_BASE + 0x6c))// 0x6c
#define GPIO_ACPU_INT1_REG      (*(volatile uint32_t*) (GPIO_BASE + 0x74))// 0x74
#define GPIO_ACPU_NMI_INT1_REG  (*(volatile uint32_t*) (GPIO_BASE + 0x78))// 0x78
#define GPIO_PCPU_INT1_REG      (*(volatile uint32_t*) (GPIO_BASE + 0x7c))// 0x7c
#define GPIO_PCPU_NMI_INT1_REG  (*(volatile uint32_t*) (GPIO_BASE + 0x80))// 0x80
#define GPIO_PINn_REG(n)        (*(volatile uint32_t*) (GPIO_BASE + 0x88 + 0x4 * (n)))


// Example: dummy HAL functions (replace with actual hardware access)
static void gpio_set_mode(uint8_t gpio, uint8_t mode) {
    if (mode == GPIO_MODE_OUTPUT) {
        GPIO_ENABLE_REG |= (1 << gpio);
    } else if (mode == GPIO_MODE_INPUT) {
        GPIO_ENABLE_REG &= ~(1 << gpio);
    }
}

void gpio_init(void) {
    gpio_set_mode(GPIO_PIN_NSS, GPIO_MODE_OUTPUT);
    gpio_set_mode(GPIO_PIN_RST, GPIO_MODE_OUTPUT);
    gpio_set_mode(GPIO_PIN_DIO0, GPIO_MODE_INPUT);
    gpio_set_mode(GPIO_PIN_DIO1, GPIO_MODE_INPUT);
    gpio_set_mode(GPIO_PIN_LED_RX, GPIO_MODE_OUTPUT);
    gpio_set_mode(GPIO_PIN_LED_TX, GPIO_MODE_OUTPUT);
}


void gpio_write(uint8_t gpio, uint8_t level) {
    if (gpio < 32) {
        if (level)
            GPIO_OUT_W1TS_REG = (1 << gpio);
        else
            GPIO_OUT_W1TC_REG = (1 << gpio);
    } else {
        if (level)
            GPIO_OUT1_W1TS_REG = (1 << (gpio - 32));
        else
            GPIO_OUT1_W1TC_REG = (1 << (gpio - 32));
    }
}

uint8_t gpio_read(uint8_t gpio) {
    if (gpio < 32) {
        return (GPIO_IN_REG >> gpio) & 0x1;
    } else {
        return (GPIO_IN1_REG >> (gpio - 32)) & 0x1;
    }
}
