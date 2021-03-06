#include <stdlib.h>
#include <string.h>
#include "cpu.h"
#include "memory.h"

// Moves the PC to the next instruction and sets the previous location of the PC.
void increment_pc(CPU *cpu)
{
    cpu->previous_pc = cpu->pc;
    cpu->pc += 2;
}

// Moves the PC to the given address and sets the previous location of the PC.
void move_pc(CPU *cpu, uint16_t address)
{
    cpu->previous_pc = cpu->pc;
    cpu->pc = address;
}

// Sets the CPU to a state where it is ready to begin executing a program.
void initialize_cpu(CPU *cpu)
{
    cpu->previous_pc = 0;
    cpu->pc = PROGRAM_OFFSET;
    cpu->sp = -1;
    cpu->dt = 0;
    cpu->st = 0;
    cpu->I = 0;
    memset(&cpu->registers, 0, 16);
    memset(&cpu->stack, 0, 16);
}

void decrement_dt(CPU *cpu)
{
    if (cpu->dt > 0)
        cpu->dt--;
}