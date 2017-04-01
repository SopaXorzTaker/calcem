#include "context.h"

#include <stdio.h>

static uint8_t dispatch_code_read(nxu8_cpu_state_t *cpu_state, size_t offs)
{
    context_t *context = (context_t *)cpu_state->userdata;
    
    return mmu_code_read(&context->mmu, offs);
}

static uint8_t dispatch_data_read(nxu8_cpu_state_t *cpu_state, size_t offs)
{
    context_t *context = (context_t *)cpu_state->userdata;
    
    return mmu_data_read(&context->mmu, offs);
}

static void dispatch_data_write(nxu8_cpu_state_t *cpu_state, size_t offs, uint8_t value)
{
    context_t *context = (context_t *)cpu_state->userdata;
    
    mmu_data_write(&context->mmu, offs, value);
}

static void dispatch_invalid_opcode_callback(nxu8_cpu_state_t *cpu_state, size_t offs)
{
    context_t *context = (context_t *)cpu_state->userdata;
    
    printf("invalid opcode at %zx\n", offs);
    
    context->running = 0;
}

void context_open(context_t *context)
{
    context->cpu_state.userdata = context;
    
    context->cpu_state.code_read = dispatch_code_read;
    context->cpu_state.data_read = dispatch_data_read;
    context->cpu_state.data_write = dispatch_data_write;
    context->cpu_state.invalid_opcode_callback = dispatch_invalid_opcode_callback;
    
    mmu_open(&context->mmu);
}

void context_close(context_t *context)
{
    mmu_close(&context->mmu);
}

