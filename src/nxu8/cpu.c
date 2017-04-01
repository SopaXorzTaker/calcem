#include "cpu.h"

#include <stdio.h>

// * note that 'i' is evaluated many times in these macros
// * example: NXU8_READ_QR(s, k++) will increment 'k' 8 times

#define  NXU8_READ_R(s, i) ((nxu8_byte)(s)->reg_r[i])
#define NXU8_READ_ER(s, i) (((( nxu8_word) NXU8_READ_R((s), (i) + 1)) <<  8) | (( nxu8_word) NXU8_READ_R((s), (i))))
#define NXU8_READ_XR(s, i) ((((nxu8_dword)NXU8_READ_ER((s), (i) + 1)) << 16) | ((nxu8_dword)NXU8_READ_ER((s), (i))))
#define NXU8_READ_QR(s, i) ((((nxu8_qword)NXU8_READ_XR((s), (i) + 1)) << 32) | ((nxu8_qword)NXU8_READ_XR((s), (i))))

#define  NXU8_WRITE_R(s, i, v) ((s)->reg_r[i] = (nxu8_byte)(v))
#define NXU8_WRITE_ER(s, i, v) ( NXU8_WRITE_R((s), (i) + 1, ( nxu8_byte)((v) >>  8)),  NXU8_WRITE_R((s), (i), ( nxu8_byte)(v)))
#define NXU8_WRITE_XR(s, i, v) (NXU8_WRITE_ER((s), (i) + 1, ( nxu8_word)((v) >> 16)), NXU8_WRITE_ER((s), (i), ( nxu8_word)(v)))
#define NXU8_WRITE_QR(s, i, v) (NXU8_WRITE_XR((s), (i) + 1, (nxu8_dword)((v) >> 32)), NXU8_WRITE_XR((s), (i), (nxu8_dword)(v)))

#define NXU8_PSW_ELEVEL ((nxu8_byte)0x03)
#define NXU8_PSW_HC     ((nxu8_byte)0x04)
#define NXU8_PSW_MIE    ((nxu8_byte)0x08)
#define NXU8_PSW_OV     ((nxu8_byte)0x10)
#define NXU8_PSW_S      ((nxu8_byte)0x20)
#define NXU8_PSW_Z      ((nxu8_byte)0x40)
#define NXU8_PSW_C      ((nxu8_byte)0x80)

void nxu8_cpu_next(nxu8_cpu_state_t *cpu_state)
{
    // * TODO: remember to process DSR instructions with the following instruction
    // * NOTE: must not return prematurely from here, important stuff happens at the end
    
    cpu_state->interrupt_accepted = 0;
}

static int nxu8_cpu_interrupt_internal(nxu8_cpu_state_t *cpu_state, size_t vector, nxu8_byte elevel)
{
    if ((cpu_state->reg_epsw[0] & NXU8_PSW_ELEVEL) > elevel)
        return 0;
    if (!(cpu_state->reg_epsw[0] & NXU8_PSW_MIE) && elevel < 2)
        return 0;
    if (cpu_state->interrupt_accepted)
        return 0;
    
    cpu_state->reg_elr[elevel] = cpu_state->reg_pc;
    cpu_state->reg_ecsr[elevel] = cpu_state->reg_csr;
    cpu_state->reg_epsw[elevel] = cpu_state->reg_epsw[0];
    
    cpu_state->reg_epsw[0] = (cpu_state->reg_epsw[0] & (~NXU8_PSW_ELEVEL)) | elevel;
    if (elevel < 2)
        cpu_state->reg_epsw[0] &= (~NXU8_PSW_MIE);
    
    cpu_state->reg_pc  = ((nxu8_word)(cpu_state->code_read(cpu_state, vector * 2    )));
    cpu_state->reg_pc |= ((nxu8_word)(cpu_state->code_read(cpu_state, vector * 2 + 1))) << 8;
    
    cpu_state->interrupt_accepted = 1;
    
    return 1;
}

int nxu8_cpu_nonmaskable_interrupt(nxu8_cpu_state_t *cpu_state, size_t vector)
{
    if (vector < 0x04 || vector > 0x1F)
        printf("warning: nxu8_cpu_nonmaskable_interrupt called with vector %zu\n", vector);
    
    return nxu8_cpu_interrupt_internal(cpu_state, vector, 2); // * vector x, elevel 2
}

int nxu8_cpu_maskable_interrupt(nxu8_cpu_state_t *cpu_state, size_t vector)
{
    if (vector < 0x04 || vector > 0x1F)
        printf("warning: nxu8_cpu_maskable_interrupt called with vector %zu\n", vector);
    
    return nxu8_cpu_interrupt_internal(cpu_state, vector, 1); // * vector x, elevel 1
}

int nxu8_cpu_software_interrupt(nxu8_cpu_state_t *cpu_state, size_t vector)
{
    if (vector < 0x20 || vector > 0x3F)
        printf("warning: nxu8_cpu_software_interrupt called with vector %zu\n", vector);
    
    return nxu8_cpu_interrupt_internal(cpu_state, vector, 1); // * vector x, elevel 1
}

int nxu8_cpu_emulator_interrupt(nxu8_cpu_state_t *cpu_state)
{
    return nxu8_cpu_interrupt_internal(cpu_state, 3, 3); // * vector 3, elevel 3
}

void nxu8_cpu_reset(nxu8_cpu_state_t *cpu_state)
{
    cpu_state->reg_sp  = ((nxu8_word)(cpu_state->code_read(cpu_state, 0)));
    cpu_state->reg_sp |= ((nxu8_word)(cpu_state->code_read(cpu_state, 1))) << 8;
    
    cpu_state->reg_pc  = ((nxu8_word)(cpu_state->code_read(cpu_state, 2)));
    cpu_state->reg_pc |= ((nxu8_word)(cpu_state->code_read(cpu_state, 3))) << 8;
    
    cpu_state->reg_epsw[0] = 0;
    cpu_state->reg_csr = 0;
    cpu_state->reg_dsr = 0;
    
    cpu_state->interrupt_accepted = 0;
    
    nxu8_cpu_interrupt_internal(cpu_state, 1, 0); // * vector 1, elevel 0
}

void nxu8_cpu_break(nxu8_cpu_state_t *cpu_state)
{
    if ((cpu_state->reg_epsw[0] & NXU8_PSW_ELEVEL) >= 2)
        nxu8_cpu_reset(cpu_state);
    else
        nxu8_cpu_interrupt_internal(cpu_state, 2, 2); // * vector 2, elevel 2
}

