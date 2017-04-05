#include "decoder.h"

#include "cpu.h"
#include "interrupt.h"

#include <stdio.h>

void nxu8_cpu_next(nxu8_cpu_state_t *cpu_state)
{
    if (cpu_state->reg_pc & 0x0001)
    {
        // * this is the user's fault, so we emit a warning
        printf("warning: LSB of PC set, correcting\n");
        cpu_state->reg_pc &= 0xFFFE;
    }
    
    nxu8_word_t instruction;
    
    int reading_dsr_prefix = 1;
    while (reading_dsr_prefix)
    {
        instruction  = ((nxu8_word_t)(cpu_state->code_read(cpu_state, cpu_state->reg_pc    )));
        instruction |= ((nxu8_word_t)(cpu_state->code_read(cpu_state, cpu_state->reg_pc + 1))) << 8;
        
        // * PC is incremented before any processing takes place,
        //   since stuff like POP might overwrite it
        // * possible overflow is intended
        cpu_state->reg_pc += 2;
        
        reading_dsr_prefix = 0; // * TODO: some clever bit banging
        if (reading_dsr_prefix)
        {
            // * TODO: update DSR
        }
    }
    
    // * process 'instruction'
    
    // * TODO: remember to process DSR instructions with the following instruction
    // * NOTE: must not return prematurely from here, important stuff happens at the end

    // * whatever happened to PC probably isn't the user's fault, no warning
    cpu_state->reg_pc &= 0xFFFE;
    
    cpu_state->interrupt_accepted = 0;
}

