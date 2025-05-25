#include "sx1278_pins.h"
#include "gpio.h"
#include "gpio_config.h"  // Contains pin definitions like GPIO_PIN_DIO0
#include "gpio_types.h"
/**
 * @brief Initializes the SX1278 GPIOs: NSS, RESET, DIOx, LEDs.
 */
void sx1278_pins_init(void) {
    gpio_configure_pin(&(gpio_config_t){ .pin = GPIO_PIN_NSS, .mode = GPIO_MODE_OUTPUT });
    gpio_configure_pin(&(gpio_config_t){ .pin = GPIO_PIN_RST, .mode = GPIO_MODE_OUTPUT });
    gpio_configure_pin(&(gpio_config_t){ .pin = GPIO_PIN_LED_RX, .mode = GPIO_MODE_OUTPUT });
    gpio_configure_pin(&(gpio_config_t){ .pin = GPIO_PIN_LED_TX, .mode = GPIO_MODE_OUTPUT });
    
    if (GPIO_PIN_DIO0 >= 0)
        gpio_configure_pin(&(gpio_config_t){ .pin = GPIO_PIN_DIO0, .mode = GPIO_MODE_INPUT });


    if (GPIO_PIN_DIO1 >= 0)
        gpio_configure_pin(&(gpio_config_t){ .pin = GPIO_PIN_DIO1, .mode = GPIO_MODE_INPUT });


    // Default states
    sx1278_nss_high();
    sx1278_reset_high();
}

/**
 * @brief Sets NSS (chip select) pin low.
 */
void sx1278_nss_low(void) {
    gpio_set_pin(GPIO_PIN_NSS,0);
}

/**
 * @brief Sets NSS (chip select) pin high.
 */
void sx1278_nss_high(void) {
    gpio_set_pin(GPIO_PIN_NSS,1);
}

/**
 * @brief Sets the RESET pin high (inactive).
 */
void sx1278_reset_high(void) {
    gpio_set_pin(GPIO_PIN_RST,1);
}

/**
 * @brief Sets the RESET pin low (active).
 */
void sx1278_reset_low(void) {
    gpio_set_pin(GPIO_PIN_RST,0);
}

/**
 * @brief Reads the current logic level of DIO0.
 * 
 * @return true if high, false otherwise.
 */
bool sx1278_dio0_read(void) {
    if (GPIO_PIN_DIO0 < 0) return false;
    return gpio_read_pin(GPIO_PIN_DIO0);
}

/**
 * @brief Reads the current logic level of DIO1.
 * 
 * @return true if high, false otherwise.
 */
bool sx1278_dio1_read(void) {
    if (GPIO_PIN_DIO1 < 0) return false;
    return gpio_readpin(GPIO_PIN_DIO1);
}
