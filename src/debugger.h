#ifndef DEBUGGER_H
#define DEBUGGER_H

#define MAX_DEBUGGER_COMMAND_ARGS 3

#include <stdbool.h>
#include "emulator.h"


typedef struct debugger_t {
    Emulator *emulator;

    // TODO: This should be dynamically allocated. Implement a resizable array?
    uint16_t break_addresses[4096];
    int break_address_count;
} Debugger;

typedef enum debugger_command_type_t {
    CONTINUE,
    EXIT,
    MEMORY,
    NEXT,
    PREVIOUS,
    REGISTER,
    REGISTERS,
    STACK,
    STEP,
    BREAK,
    HELP,
    CONTEXT
} DebuggerCommandType;

typedef struct debugger_command_t {
    DebuggerCommandType type;
    char **args;
    int arg_count;
} DebuggerCommand;

int parse_debugger_command(char *input, DebuggerCommand *command);
int destroy_debugger_command(DebuggerCommand *command);
void execute_debugger_command(Debugger *debugger, DebuggerCommand *command);
bool should_break(Debugger *debugger);

#endif