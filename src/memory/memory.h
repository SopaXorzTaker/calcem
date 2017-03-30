#ifndef MEMORY__MEMORY_H
#define MEMORY__MEMORY_H

#include "../config.h"

typedef struct memory_s_
{
    fxes_byte *code, *data;
} memory_t;

void memory_open(memory_t *memory);
void memory_close(memory_t *memory);

#endif // MEMORY__MEMORY_H

