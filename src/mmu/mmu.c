#include "mmu.h"

#include <assert.h>

void mmu_open(mmu_t *mmu)
{
    mmu->code = malloc(sizeof(uint8_t) * mmu->code_size);
    assert(mmu->code);
    
    mmu->data = malloc(sizeof(uint8_t) * mmu->data_size);
    assert(mmu->data);
}

void mmu_close(mmu_t *mmu)
{
    free(mmu->data);
    free(mmu->code);
}

uint8_t mmu_code_read(mmu_t *mmu, size_t offs)
{
    return mmu->code[offs];
}

uint8_t mmu_data_read(mmu_t *mmu, size_t offs)
{
    return mmu->data[offs];
}

void mmu_data_write(mmu_t *mmu, size_t offs, uint8_t value)
{
    mmu->data[offs] = value;
}



