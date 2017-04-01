#ifndef NXU8__CPU_H
#define NXU8__CPU_H

#include <stdlib.h>
#include <stdint.h>

typedef struct nxu8_cpu_state_s_ nxu8_cpu_state_t;

typedef uint8_t nxu8_byte;
typedef uint16_t nxu8_word;
typedef uint32_t nxu8_dword;
typedef uint64_t nxu8_qword;

typedef nxu8_byte (*nxu8_code_read_proc)(nxu8_cpu_state_t *cpu_state, size_t offs);
typedef nxu8_byte (*nxu8_data_read_proc)(nxu8_cpu_state_t *cpu_state, size_t offs);
typedef void (*nxu8_data_write_proc)(nxu8_cpu_state_t *cpu_state, size_t offs, nxu8_byte value);
typedef void (*nxu8_invalid_opcode_callback_proc)(nxu8_cpu_state_t *cpu_state, size_t offs);

typedef struct nxu8_cpu_state_s_
{
    nxu8_byte reg_r[16];
    
    nxu8_word reg_sp, reg_ea, reg_dsr;
    nxu8_word reg_pc, reg_elr[4]; // * lr is elr[0]
    nxu8_byte reg_csr, reg_ecsr[4]; // * lcsr is ecsr[0]
    nxu8_byte reg_epsw[4]; // * pws is epsw[0]
    
    nxu8_code_read_proc code_read;
    nxu8_data_read_proc data_read;
    nxu8_data_write_proc data_write;
    nxu8_invalid_opcode_callback_proc invalid_opcode_callback;
    
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
