#ifndef MMU__MMU_H
#define MMU__MMU_H

#include <stdint.h>
#include <stdlib.h>

typedef struct _avl_tree_s avl_tree_t;

typedef uint8_t (*mmu_hook_receiver_read_callback_func_t)(void *userdata, size_t offs);
typedef void (*mmu_hook_receiver_write_callback_func_t)(void *userdata, size_t offs, uint8_t value);

typedef struct mmu_hook_receiver_s_
{
    mmu_hook_receiver_read_callback_func_t read;
    mmu_hook_receiver_write_callback_func_t write;
} mmu_hook_receiver_t;

typedef struct mmu_hook_s_
{
    size_t base, size;
    mmu_hook_receiver_t *hook_receiver;
    void *userdata;
} mmu_hook_t;

typedef struct mmu_s_
{
    uint8_t *code, *data;
    size_t code_size, data_size;
    avl_tree_t *hooks;
} mmu_t;

void mmu_open(mmu_t *mmu);
void mmu_close(mmu_t *mmu);
uint8_t mmu_code_read(mmu_t *mmu, size_t offs);
uint8_t mmu_data_read(mmu_t *mmu, size_t offs);
void mmu_data_write(mmu_t *mmu, size_t offs, uint8_t value);

#endif // MMU__MMU_H

