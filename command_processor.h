#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include <stdio.h>
#include <stdint.h>

// Command types
typedef enum {
    CMD_INSERT,
    CMD_DELETE,
    CMD_SEARCH,
    CMD_PRINT
} CommandType;

// Command structure
typedef struct {
    CommandType type;
    char name[50];
    uint32_t salary;
} Command;

// Thread arguments
typedef struct {
    Command cmd;
    FILE *outputFile;
} ThreadArgs;

// Function prototypes
void parseCommand(char *line, Command *cmd);
void *processCommand(void *arg);

#endif // COMMAND_PROCESSOR_H
