#ifndef NXU8__CPU_H
#define NXU8__CPU_H

#include <stdint.h>

#include <stdlib.h>
#include "../debug/mmdebug.h"

typedef struct nxu8_cpu_state_s_ nxu8_cpu_state_t;

typedef uint8_t nxu8_byte_t;
typedef uint16_t nxu8_word_t;
typedef uint32_t nxu8_dword_t;
typedef uint64_t nxu8_qword_t;

typedef nxu8_byte_t (*nxu8_code_read_t)(nxu8_cpu_state_t *cpu_state, size_t offs);
typedef nxu8_byte_t (*nxu8_data_read_t)(nxu8_cpu_state_t *cpu_state, size_t offs);
typedef void (*nxu8_data_write_t)(nxu8_cpu_state_t *cpu_state, size_t offs, nxu8_byte_t value);
typedef void (*nxu8_invalid_opcode_callback_t)(nxu8_cpu_state_t *cpu_state, size_t offs);
typedef nxu8_byte_t (*nxu8_coproc_reg_read_t)(nxu8_cpu_state_t *cpu_state, size_t regn);
typedef void (*nxu8_coproc_reg_write_t)(nxu8_cpu_state_t *cpu_state, size_t regn, nxu8_byte_t value);

typedef struct nxu8_cpu_state_s_
{
    nxu8_byte_t reg_r[16];
    
    nxu8_word_t reg_sp, reg_ea, reg_dsr;
    nxu8_word_t reg_pc, reg_elr[4]; // * lr is elr[0]
    nxu8_byte_t reg_csr, reg_ecsr[4]; // * lcsr is ecsr[0]
    nxu8_byte_t reg_epsw[4]; // * pws is epsw[0]
    
    nxu8_code_read_t code_read;
    nxu8_data_read_t data_read;
    nxu8_data_write_t data_write;
    nxu8_invalid_opcode_callback_t invalid_opcode_callback;
    
    nxu8_coproc_reg_read_t coproc_reg_read;
    nxu8_coproc_reg_write_t coproc_reg_write;
    
    int interrupt_accepted;
    
    void *userdata;
} nxu8_cpu_state_t;

void nxu8_cpu_next(nxu8_cpu_state_t *cpu_state);
int nxu8_cpu_nonmaskable_interrupt(nxu8_cpu_state_t *cpu_state, size_t vector);
int nxu8_cpu_maskable_interrupt(nxu8_cpu_state_t *cpu_state, size_t vector);
int nxu8_cpu_software_interrupt(nxu8_cpu_state_t *cpu_state, size_t vector);
int nxu8_cpu_emulator_interrupt(nxu8_cpu_state_t *cpu_state);
void nxu8_cpu_reset(nxu8_cpu_state_t *cpu_state);
void nxu8_cpu_break(nxu8_cpu_state_t *cpu_state);

#endif // NXU8__CPU_H

