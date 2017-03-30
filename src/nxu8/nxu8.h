#ifndef NXU8__NXU8_H
# define NXU8__NXU8_H

#include <stdint.h>

typedef uint8_t nxu8_byte;
typedef uint16_t nxu8_word;
typedef uint32_t nxu8_dword;
typedef uint64_t nxu8_qword;

typedef nxu8_byte (*nxu8_code_read_proc)(size_t offs);
typedef nxu8_byte (*nxu8_data_read_proc)(size_t offs);
typedef void (*nxu8_data_write_proc)(size_t offs, nxu8_byte value);

typedef struct nxu8_cpu_state_s_
{
    union
    {
        nxu8_byte reg_r[16];
        nxu8_word reg_er[8];
        nxu8_dword reg_xr[4];
        nxu8_qword reg_qr[2];
    };
    
    nxu8_word reg_sp, reg_ea, reg_dsr;
    nxu8_word reg_pc, reg_elr[4]; // * lr is elr[0]
    nxu8_byte reg_csr, reg_ecsr[4]; // * lcsr is ecsr[0]
    nxu8_byte reg_epsw[4]; // * pws is epsw[0]
    
    nxu8_code_read_proc code_read;
    nxu8_data_read_proc data_read;
    nxu8_data_write_proc data_write;
} nxu8_cpu_state_t;

#endif // NXU8__NXU8_H

