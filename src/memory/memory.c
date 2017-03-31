#include "memory.h"

#include <assert.h>

void memory_open(memory_t *memory, size_t code_size, size_t data_size)
{
    memory->code_size = code_size;
    memory->code = malloc(sizeof(uint8_t) * code_size);
    assert(memory->code);
    
    memory->data_size = data_size;
    memory->data = malloc(sizeof(uint8_t) * data_size);
    assert(memory->data);
}

void memory_close(memory_t *memory)
{
    free(memory->data);
    free(memory->code);
}


