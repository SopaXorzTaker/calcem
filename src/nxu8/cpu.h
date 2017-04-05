#ifndef NXU8__CPU_H
#define NXU8__CPU_H

#include <stdint.h>
#include <stdlib.h>

// * note that 'i' is evaluated many times in these macros
// * example: NXU8_READ_QR(s, k++) will increment 'k' 8 times

#define NXU8_READ_R (s, i) ((nxu8_byte_t)(s)->reg_r[i])
#define NXU8_READ_ER(s, i) (((( nxu8_word_t) NXU8_READ_R((s), (i) + 1)) <<  8) | (( nxu8_word_t) NXU8_READ_R((s), (i))))
#define NXU8_READ_XR(s, i) ((((nxu8_dword_t)NXU8_READ_ER((s), (i) + 1)) << 16) | ((nxu8_dword_t)NXU8_READ_ER((s), (i))))
#define NXU8_READ_QR(s, i) ((((nxu8_qword_t)NXU8_READ_XR((s), (i) + 1)) << 32) | ((nxu8_qword_t)NXU8_READ_XR((s), (i))))

#define NXU8_WRITE_R (s, i, v) ((s)->reg_r[i] = (nxu8_byte_t)(v))
#define NXU8_WRITE_ER(s, i, v) ( NXU8_WRITE_R((s), (i) + 1, ( nxu8_byte_t)((v) >>  8)),  NXU8_WRITE_R((s), (i), ( nxu8_byte_t)(v)))
#define NXU8_WRITE_XR(s, i, v) (NXU8_WRITE_ER((s), (i) + 1, ( nxu8_word_t)((v) >> 16)), NXU8_WRITE_ER((s), (i), ( nxu8_word_t)(v)))
#define NXU8_WRITE_QR(s, i, v) (NXU8_WRITE_XR((s), (i) + 1, (nxu8_dword_t)((v) >> 32)), NXU8_WRITE_XR((s), (i), (nxu8_dword_t)(v)))

#define NXU8_READ_CR (s, i) ((s)->coproc_reg_read((s), (i)))
#define NXU8_READ_CER(s, i) (((( nxu8_word_t) NXU8_READ_CR((s), (i) + 1)) <<  8) | (( nxu8_word_t) NXU8_READ_CR((s), (i))))
#define NXU8_READ_CXR(s, i) ((((nxu8_dword_t)NXU8_READ_CER((s), (i) + 1)) << 16) | ((nxu8_dword_t)NXU8_READ_CER((s), (i))))
#define NXU8_READ_CQR(s, i) ((((nxu8_qword_t)NXU8_READ_CXR((s), (i) + 1)) << 32) | ((nxu8_qword_t)NXU8_READ_CXR((s), (i))))

#define NXU8_WRITE_CR (s, i, v) ((s)->coproc_reg_write((s), (i), (nxu8_byte_t)(v)))
#define NXU8_WRITE_CER(s, i, v) ( NXU8_WRITE_CR((s), (i) + 1, ( nxu8_byte_t)((v) >>  8)),  NXU8_WRITE_CR((s), (i), ( nxu8_byte_t)(v)))
#define NXU8_WRITE_CXR(s, i, v) (NXU8_WRITE_CER((s), (i) + 1, ( nxu8_word_t)((v) >> 16)), NXU8_WRITE_CER((s), (i), ( nxu8_word_t)(v)))
#define NXU8_WRITE_CQR(s, i, v) (NXU8_WRITE_CXR((s), (i) + 1, (nxu8_dword_t)((v) >> 32)), NXU8_WRITE_CXR((s), (i), (nxu8_dword_t)(v)))

#define NXU8_PSW_ELEVEL ((nxu8_byte_t)0x03)
#define NXU8_PSW_HC     ((nxu8_byte_t)0x04)
#define NXU8_PSW_MIE    ((nxu8_byte_t)0x08)
#define NXU8_PSW_OV     ((nxu8_byte_t)0x10)
#define NXU8_PSW_S      ((nxu8_byte_t)0x20)
#define NXU8_PSW_Z      ((nxu8_byte_t)0x40)
#define NXU8_PSW_C      ((nxu8_byte_t)0x80)

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

#endif // NXU8__CPU_H

