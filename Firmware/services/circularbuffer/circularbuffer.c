#include "circularbuffer.h"

#include <stdlib.h>


typedef struct CircularBuffer {
    uint8_t *data;  // Pointer to dynamically allocated buffer
    uint16_t size;    // Buffer capacity
    uint16_t head;    // Write position
    uint16_t tail;    // Read position
    uint16_t count;   // Number of elements in buffer
} CircularBuffer_t;

CircularBuffer_t* create_buffer(CircularBuffer_t *buffer, uint8_t bufferSize) {
    CircularBuffer_t *cb = (CircularBuffer_t*) malloc(sizeof(CircularBuffer_t));
    if (cb == NULL) return NULL;  // Allocation failed

    cb->data= (uint8_t*) malloc(bufferSize * sizeof(uint8_t));
    if (cb->data== NULL) {
        free(cb);  // Free struct if allocation fails
        return NULL;
    }

    cb->size = bufferSize;
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;

    return cb;
}

void clear_buffer(CircularBuffer_t *cb) {
    free(cb->data);
    free(cb);
}

void write_buffer(CircularBuffer_t *cb, uint8_t data, bool ov) {
    if (cb->count < cb->size) {
        cb->data[cb->head] = data;
        cb->head = (cb->head + 1) % cb->size;
        cb->count++;
    }else if (ov) {
        // Overwrite oldes data
        cb->data[cb->head] = data;
        cb->head = (cb->head + 1) % cb->size;
        cb->tail = (cb->tail + 1) % cb->size;  // Move tail forward
    }
}

uint8_t read_buffer(CircularBuffer_t *cb) {
    if (cb->count > 0) {
        uint8_t data = cb->data[cb->tail];
        cb->tail = (cb->tail+1) % cb->size;
        cb->count--;
        return data;
    }
    return 0;
}


bool buffer_is_empty(const CircularBuffer_t* buffer) {
    return buffer->size == 0;
}


bool buffer_is_full(const CircularBuffer_t* buffer) {
    return buffer->size == buffer->count;
}


uint8_t buffer_peek(const CircularBuffer_t* buffer) {
    if (buffer->count == 0) {return 0;}
    return buffer->data[buffer->tail];
}


uint8_t buffer_size(const CircularBuffer_t* buffer) {
    return buffer->count;
}

uint8_t buffer_space_left(const CircularBuffer_t* buffer) {
    return buffer->size - buffer->count;
}
