#pragma once 

#include <stdint.h>
#include <stdbool.h>

typedef struct CircularBuffer CircularBuffer_t;

/**
 * @brief Initializes and allocates memory for a circular buffer.
 * 
 * @param buffer Pointer to the CircularBuffer_t struct to be initialized.
 * @param bufferSize The size of the buffer in bytes.
 * @return CircularBuffer_t* Pointer to the initialized buffer, or NULL if allocation fails.
 * 
 * @note If the buffer already contains allocated memory, it should be cleared before calling this function.
 */
CircularBuffer_t* create_buffer(CircularBuffer_t *buffer, uint8_t bufferSize);

/**
 * @brief Clears the circular buffer and resets all positions.
 * 
 * @param buffer Pointer to the CircularBuffer_t struct to be cleared.
 * 
 * @note This function does not free the buffer memory but resets its head, tail, and count to zero.
 */
void clear_buffer(CircularBuffer_t *buffer);

/**
 * @brief Writes a byte of data into the circular buffer.
 * 
 * @param buffer Pointer to the CircularBuffer_t struct.
 * @param data The byte of data to store in the buffer.
 * @param ov Boolean flag indicating whether to overwrite the oldest data if the buffer is full.
 * 
 * @note If the buffer is full and `ov` is false, the function will discard new data.
 * @note If `ov` is true and the buffer is full, the oldest data will be overwritten.
 */
void write_buffer(CircularBuffer_t *buffer, uint8_t data, bool ov);

/**
 * @brief Reads a byte of data from the circular buffer.
 * 
 * @param buffer Pointer to the CircularBuffer_t struct.
 * @return uint8_t The oldest byte in the buffer, or 0 if the buffer is empty.
 * 
 * @note If the buffer is empty, the function returns 0.
 * @note After reading, the tail index is incremented to point to the next available data.
 */
uint8_t read_buffer(CircularBuffer_t *buffer);