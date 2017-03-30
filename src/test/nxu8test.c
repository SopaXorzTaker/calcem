#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "../nxu8/cpu.h"
#include "../memory/memory.h"

int main(int argc, char *argv[])
{
    nxu8_cpu_state_t nxu8_cpu_state;
    memory_t memory;
    
    if (argc < 2)
    {
        printf("no code dump passed\n");
        return 1;
    }
    
    FILE *handle = fopen(argv[1], "rb");
    if (!handle)
    {
        printf("failed to open '%s' for reading\n", argv[1]);
        return 2;
    }
    
    memory_open(&memory); // * alloc memory here; at this point fread should work
    
    size_t code_size = fread(memory.code, sizeof(fxes_byte), FXES_CODE_SIZE, handle);
    fclose(handle);
    printf("Read %zu bytes of code\n", code_size);
    
    
    
    
    memory_close(&memory); // * free memory here
    
    return 0;
}

