#include "memory.h"

#include <stdlib.h>
#include <assert.h>

void memory_open(memory_t *memory)
{
    memory->code = malloc(sizeof(fxes_byte) * FXES_CODE_SIZE);
    assert(memory->code);
    
    memory->data = malloc(sizeof(fxes_byte) * FXES_DATA_SIZE);
    assert(memory->data);
}

void memory_close(memory_t *memory)
{
    free(memory->data);
    free(memory->code);
}


