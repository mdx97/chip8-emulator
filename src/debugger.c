#include <stdlib.h>
#include <stdint.h>
#include "debugger.h"
#include "memory.h"

void print_register_values(CPU *cpu)
{
    printf("I: 0x%x\n", cpu->I);
    printf("dt: 0x%x\n", cpu->dt);
    printf("st: 0x%x\n", cpu->st);
    for (int i = 0; i < 16; i++)
        printf("V%d: 0x%x\n", i, cpu->registers[i]);
}

/*
 * Attempts to parse debugger prompt input into a DebuggerCommand struct.
 * 
 * Returns:
 * 0 - Success.
 * 1 - Invalid command.
 * 2 - Too many arguments (cannot fit inside fixed size command.args buffer).
 */
int parse_debugger_command(char *input, DebuggerCommand *command)
{
    char *token = strtok(input, " ");

    if (strcmp(input, "continue") == 0)
        command->type = CONTINUE;
    else if (strcmp(input, "exit") == 0)
        command->type = EXIT;
    else if (strcmp(input, "memory") == 0)
        command->type = MEMORY;
    else if (strcmp(input, "next") == 0)
        command->type = NEXT;
    else if (strcmp(input, "previous") == 0)
        command->type = PREVIOUS;
    else if (strcmp(input, "register") == 0)
        command->type = REGISTER;
    else if (strcmp(input, "registers") == 0)
        command->type = REGISTERS;
    else if (strcmp(input, "step") == 0)
        command->type = STEP;
    else
        return 1;
    
    int i = 0;
    command->args = (char **)malloc(MAX_DEBUGGER_COMMAND_ARGS * sizeof(char *));
    token = strtok(NULL, " ");

    while (token != NULL) {
        if (i >= MAX_DEBUGGER_COMMAND_ARGS)
            return 2;
        command->args[i] = token;
        token = strtok(NULL, " ");
        i++;
    }

    command->arg_count = i;
    return 0;
}

// Executes a debugger command against an Emulator instance.
void execute_debugger_command(DebuggerCommand *command, Emulator *emulator)
{
    if (command->type == CONTINUE) {
        printf("Not supported yet.\n");
    } else if (command->type == STEP) {
        execute_next_instruction(emulator);
    } else if (command->type == MEMORY) {
        if (command->arg_count < 2) {
            printf("Usage: memory <start address> <end address> [chunk size]\n");
            return;
        }

        // FIXME: Need to do better error handling here.
        // Does not currently account for junk values.
        uint16_t start = (uint16_t)strtol(command->args[0], NULL, 0);
        uint16_t end = (uint16_t)strtol(command->args[1], NULL, 0);

        int bytes_per = 1;

        if (command->arg_count >= 3) {
            if (strcmp(command->args[2], "2") == 0) bytes_per = 2;
            else if (strcmp(command->args[2], "4") == 0) bytes_per = 4;
        }

        if (start < 0 || (end + bytes_per) >= MEMORY_SIZE) {
            printf("The memory range specified is outside the bounds of the memory!\n");
            return;
        }

        for (int i = start; i <= end; i += bytes_per) {
            unsigned int memory_value = emulator->memory.values[i];
            for (int j = 1; j < bytes_per; j++) {
                memory_value <<= 8;
                memory_value |= emulator->memory.values[i + j];
            }
            printf("[0x%x] 0x%x\n", i, memory_value);
        }
    } else if (command->type == NEXT) {
        printf("0x%x\n", next_instruction(emulator));
    } else if (command->type == PREVIOUS) {
        printf("0x%x\n", previous_instruction(emulator));
    } else if (command->type == REGISTER) {
        if (strcmp(command->args[0], "I") == 0)
            printf("I: 0x%x\n", emulator->cpu.I);
        else if (strcmp(command->args[0], "dt") == 0)
            printf("dt: 0x%x\n", emulator->cpu.dt);
        else if (strcmp(command->args[0], "st") == 0)
            printf("st: 0x0%x\n", emulator->cpu.st);
        else {
            // FIXME: Need to do better error handling here.
            // Does not currently account for junk values.
            int v = atoi(command->args[0]);
            if (v < 0 || v > 15) {
                printf("%d is not a valid register number!\n", v);
                return;
            }
            printf("V%d: 0x%x\n", v, emulator->cpu.registers[v]);
        }
    } else if (command->type == REGISTERS) {
        print_register_values(&(emulator->cpu));
    }
}

// Frees all memory allocated by parse_debugger_command.
int destroy_debugger_command(DebuggerCommand *command)
{
    free(command->args);
}