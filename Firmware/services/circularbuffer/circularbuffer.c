#include "circularbuffer.h"

#include <stdio.h>
#include "configuration.h"

typedef struct CircularBuffer {
    uint8_t *buffer;  // Pointer to dynamically allocated buffer
    uint16_t size;    // Buffer capacity
    uint16_t head;    // Write position
    uint16_t tail;    // Read position
    uint16_t count;   // Number of elements in buffer
} CircularBuffer_t;

CircularBuffer_t* CreateBuffer(CircularBuffer_t *buffer, uint8_t bufferSize) {
    CircularBuffer_t *cb = (CircularBuffer_t*) malloc(sizeof(CircularBuffer_t));
    if (cb == NULL) return NULL;  // Allocation failed

    cb->buffer = (uint8_t*) malloc(bufferSize * sizeof(uint8_t));
    if (cb->buffer == NULL) {
        free(cb);  // Free struct if allocation fails
        return NULL;
    }

    cb->size = bufferSize;
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;

    return cb;
}

void ClearBuffer(CircularBuffer_t *cb) {
    free(cb->buffer);
    free(cb);
}

void WriteBuffer(CircularBuffer_t *cb, uint8_t data, bool ov) {
    if (cb->count < cb->size) {
        cb->buffer[cb->head] = data;
        cb->head = (cb->head + 1) % cb->size;
        cb->count++;
    }else if (ov) {
        // Overwrite oldes data
        cb->buffer[cb->head] = data;
        cb->head = (cb->head + 1) % cb->size;
        cb->tail = (cb->tail + 1) % cb->size;  // Move tail forward
    }
}

uint8_t ReadBuffer(CircularBuffer_t *cb) {
    if (cb->count > 0) {
        uint8_t data = cb->buffer[cb->tail];
        cb->tail = (cb->tail+1) % cb->size;
        cb->count--;
        return data;
    }
    return 0;
}
