#ifndef MEMORY__MEMORY_H
#define MEMORY__MEMORY_H

#include <stdint.h>
#include <stdlib.h>

typedef struct memory_s_
{
    uint8_t *code, *data;
    size_t code_size, data_size;
} memory_t;

void memory_open(memory_t *memory, size_t code_size, size_t data_size);
void memory_close(memory_t *memory);

#endif // MEMORY__MEMORY_H

