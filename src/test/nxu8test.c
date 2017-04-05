#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include <stdlib.h>
#include "../debug/mmdebug.h"

#include "context.h"

#include "../nxu8/cpu.h"
#include "../nxu8/decoder.h"
#include "../nxu8/interrupt.h"

#define FXES_CODE_SIZE ((1 << 16) << 4)
#define FXES_DATA_SIZE ((1 << 16) << 8)

int main(int argc, char *argv[])
{
    MMDEBUG_INIT;
    
    if (argc < 2)
    {
        printf("no code dump passed\n");
        return 1;
    }
    
    context_t context;
    context.running = 1;
    context.mmu.code_size = FXES_CODE_SIZE;
    context.mmu.data_size = FXES_DATA_SIZE;
    context_open(&context);
    
    {
        FILE *handle = fopen(argv[1], "rb");
        if (!handle)
        {
            printf("failed to open '%s' for reading\n", argv[1]);
            return 2;
        }
        size_t code_source_size = fread(context.mmu.code, sizeof(uint8_t), FXES_CODE_SIZE, handle);
        fclose(handle);
        printf("Read %zu bytes of code\n", code_source_size);
    }
    
    nxu8_cpu_reset(&context.cpu_state);
    while (context.running)
    {
        nxu8_cpu_next(&context.cpu_state);
    }
    
    context_close(&context);
    
    MMDEBUG_EXIT;
    return 0;
}

