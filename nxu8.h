#ifndef NXU8_H
#define NXU8_H

#include <stdint.h>

#define NXU8_BYTE uint8_t
#define NXU8_WORD uint16_t
#define NXU8_DWORD uint32_t
#define NXU8_QWORD uint64_t

#define NXU8_BREG_R(state,n) (state->regs[n])
#define NXU8_WREG_R(state,n) (NXU8_BREG_R(state,n)|(NXU8_BREG_R(state,n+1)<<8))
#define NXU8_DREG_R(state,n) (NXU8_WREG_R(state,n)|(NXU8_WREG_R(state,n+2)<<16))
#define NXU8_QREG_R(state,n) (NXU8_DREG_R(state,n)|(NXU8_DREG_R(state,n+4)<<32))

#define NXU8_BREG_W(state,n,x) (state->regs[n]=x)
#define NXU8_WREG_W(state,n,x) (NXU8_BREG_W(state,n,x&0xFF);NXU8_BREG_W(state,n+1,(x>>8)&0xFF))
#define NXU8_DREG_W(state,n,x) (NXU8_WREG_W(state,n,x&0xFFFF);NXU8_WREG_W(state,n+2,(x>>16)&0xFFFF))
#define NXU8_QREG_W(state,n,x) (NXU8_DREG_W(state,n,x&0xFFFFFFFF);NXU8_DREG_W(state,n+4,(x>>32)&0xFFFFFFFF))

#define NXU8_FLAG_C 128
#define NXU8_FLAG_Z 64
#define NXU8_FLAG_S 32
#define NXU8_FLAG_OV 16
#define NXU8_FLAG_MIE 8
#define NXU8_FLAG_HC 4

#define NXU8_LR_R(state) ((state->psw&3)==0?state->lr:state->elr[(state->psw&3)-1])
#define NXU8_CSR_R(state) ((state->psw&3)==0?state->csr:state->ecsr[(state->psw&3)-1])
#define NXU8_PSW_R(state) ((state->psw&3)==0?state->psw:state->epsw[(state->psw&3)-1])

#define NXU8_LR_W(state,x) ((state->psw&3)==0?(state->lr=x):(state->elr[(state->psw&3)-1]=x))
#define NXU8_CSR_W(state,x) ((state->psw&3)==0?(state->csr=x):(state->ecsr[(state->psw&3)-1]=x))
#define NXU8_PSW_W(state,x) ((state->psw&3)==0?(state->psw=x):(state->epsw[(state->psw&3)-1]=x))

#define NXU8_PSW_S(state,f) (NXU8_PSW_W(state,NXU8_PSW_R(state)|f))
#define NXU8_PSW_C(state,f) (NXU8_PSW_W(state,NXU8_PSW_R(state)&(0xFF^f)))
#define NXU8_PSW_F(state,f) (NXU8_PSW_R(state)&f?1:0)

#define NXU8_PC_R(state) (state->pc&0xFFFE)
#define NXU8_PC_W(state,x) (state->pc=x&0xFFFE)

typedef struct {
    NXU8_BYTE regs[16];

    NXU8_WORD pc;
    NXU8_BYTE csr;
    NXU8_BYTE psw;
    NXU8_WORD sp;

    NXU8_WORD lr;
    NXU8_WORD elr[3];
    NXU8_BYTE lcsr;
    NXU8_BYTE ecsr[3];
    NXU8_BYTE epsw[3];
    NXU8_WORD ea;
    NXU8_WORD ar;
} nxu8_state;

#endif
