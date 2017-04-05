#ifndef NXU8__INTERRUPT_H
#define NXU8__INTERRUPT_H

#include <stdlib.h>

typedef struct nxu8_cpu_state_s_ nxu8_cpu_state_t;

int nxu8_cpu_nonmaskable_interrupt(nxu8_cpu_state_t *cpu_state, size_t vector);
int nxu8_cpu_maskable_interrupt(nxu8_cpu_state_t *cpu_state, size_t vector);
int nxu8_cpu_software_interrupt(nxu8_cpu_state_t *cpu_state, size_t vector);
int nxu8_cpu_emulator_interrupt(nxu8_cpu_state_t *cpu_state);
void nxu8_cpu_reset(nxu8_cpu_state_t *cpu_state);
void nxu8_cpu_break(nxu8_cpu_state_t *cpu_state);

#endif // NXU8__INTERRUPT_H

