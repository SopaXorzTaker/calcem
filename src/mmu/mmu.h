#ifndef MMU__MMU_H
#define MMU__MMU_H

#include <stdint.h>
#include <stdlib.h>

typedef struct mmu_s_
{
    uint8_t *code, *data;
    size_t code_size, data_size;
} mmu_t;

void mmu_open(mmu_t *mmu);
void mmu_close(mmu_t *mmu);
uint8_t mmu_code_read(mmu_t *mmu, size_t offs);
uint8_t mmu_data_read(mmu_t *mmu, size_t offs);
void mmu_data_write(mmu_t *mmu, size_t offs, uint8_t value);

#endif // MMU__MMU_H

