#include "sx1278_irq.h"
#include "irq.h"
#include "gpio.h"
#include "gpio_config.h" // Defines GPIO_PIN_DIO0, GPIO_PIN_DIO1

static sx1278_irq_callback_t dio0_callback = NULL;
static sx1278_irq_callback_t dio1_callback = NULL;

/**
 * @brief Internal ISR function for DIO0.
 */
static void IRAM_ATTR dio0_isr(void *arg) {
    (void)arg;
    sx1278_irq_dio0_handler();
}

/**
 * @brief Internal ISR function for DIO1.
 */
static void IRAM_ATTR dio1_isr(void *arg) {
    (void)arg;
    sx1278_irq_dio1_handler();
}

/**
 * @brief Initializes interrupt pins and internal callback system.
 * 
 * Call this once during driver init.
 */
void sx1278_irq_init(void) {
    // Configure DIO0 as input and attach IRQ
    gpio_set_mode(GPIO_PIN_DIO0, GPIO_MODE_INPUT);
    irq_attach_gpio(GPIO_PIN_DIO0, GPIO_INT_RISING_EDGE, dio0_isr, NULL);

#if GPIO_PIN_DIO1 >= 0
    gpio_set_mode(GPIO_PIN_DIO1, GPIO_MODE_INPUT);
    irq_attach_gpio(GPIO_PIN_DIO1, GPIO_INT_RISING_EDGE, dio1_isr, NULL);
#endif
}

/**
 * @brief Registers a user-defined callback for DIO0 (usually TxDone/RxDone).
 */
void sx1278_irq_register_dio0(sx1278_irq_callback_t cb) {
    dio0_callback = cb;
}

/**
 * @brief Registers a user-defined callback for DIO1 (optional, e.g. RxTimeout).
 */
void sx1278_irq_register_dio1(sx1278_irq_callback_t cb) {
    dio1_callback = cb;
}

/**
 * @brief Called internally by ISR when DIO0 fires.
 */
void sx1278_irq_dio0_handler(void) {
    if (dio0_callback) {
        dio0_callback();
    }
}

/**
 * @brief Called internally by ISR when DIO1 fires.
 */
void sx1278_irq_dio1_handler(void) {
    if (dio1_callback) {
        dio1_callback();
    }
}
