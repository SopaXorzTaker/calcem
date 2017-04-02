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
}

static uint8_t dispatch_coproc_reg_read(nxu8_cpu_state_t *cpu_state, size_t regn)
{
    context_t *context = (context_t *)cpu_state->userdata;
    
    return context->coproc_reg[regn];
}

static void dispatch_coproc_reg_write(nxu8_cpu_state_t *cpu_state, size_t regn, uint8_t value)
{
    context_t *context = (context_t *)cpu_state->userdata;
    
    context->coproc_reg[regn] = value;
}

void context_open(context_t *context)
{
    context->cpu_state.userdata = context;
    
    context->cpu_state.code_read = dispatch_code_read;
    context->cpu_state.data_read = dispatch_data_read;
    context->cpu_state.data_write = dispatch_data_write;
    context->cpu_state.invalid_opcode_callback = dispatch_invalid_opcode_callback;
    context->cpu_state.coproc_reg_read = dispatch_coproc_reg_read;
    context->cpu_state.coproc_reg_write = dispatch_coproc_reg_write;
    
    mmu_open(&context->mmu);
}

void context_close(context_t *context)
{
    mmu_close(&context->mmu);
}

