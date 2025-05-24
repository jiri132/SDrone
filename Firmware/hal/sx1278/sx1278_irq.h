#pragma once 

/**
 * @brief Type for SX1278 DIOx interrupt callbacks.
 */
typedef void (*sx1278_irq_callback_t)(void);

/**
 * @brief Initializes interrupt pins and internal callback system.
 * 
 * Call this once during driver init.
 */
void sx1278_irq_init(void);

/**
 * @brief Registers a user-defined callback for DIO0 (usually TxDone/RxDone).
 * 
 * @param cb Callback function to invoke on DIO0 rising edge.
 */
void sx1278_irq_register_dio0(sx1278_irq_callback_t cb);

/**
 * @brief Registers a user-defined callback for DIO1 (optional, e.g. RxTimeout).
 * 
 * @param cb Callback function to invoke on DIO1 rising edge.
 */
void sx1278_irq_register_dio1(sx1278_irq_callback_t cb);

/**
 * @brief To be called from your GPIO ISR when DIO0 fires.
 */
void sx1278_irq_dio0_handler(void);

/**
 * @brief To be called from your GPIO ISR when DIO1 fires.
 */
void sx1278_irq_dio1_handler(void);
