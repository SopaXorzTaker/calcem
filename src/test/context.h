#ifndef TEST__CONTEXT_H
#define TEST__CONTEXT_H

#include "../mmu/mmu.h"
#include "../nxu8/cpu.h"

typedef struct context_s_
{
    nxu8_cpu_state_t cpu_state;
    mmu_t mmu;
    
    int running;
} context_t;

void context_open(context_t *context);
void context_close(context_t *context);

#endif // TEST__CONTEXT_H

