#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "command_processor.h"
#include "hash_table.h"

// Parse a command line
void parseCommand(char *line, Command *cmd) {
    char cmdStr[20];
    char nameBuffer[100];
    uint32_t value;
    
    // Remove newline if present
    size_t len = strlen(line);
    if (len > 0 && line[len-1] == '\n') {
        line[len-1] = '\0';
    }
    
    // Split by comma
    sscanf(line, "%[^,],%[^,],%u", cmdStr, nameBuffer, &value);
    
    // Determine command type
    if (strcmp(cmdStr, "insert") == 0) {
        cmd->type = CMD_INSERT;
        strncpy(cmd->name, nameBuffer, sizeof(cmd->name) - 1);
        cmd->name[sizeof(cmd->name) - 1] = '\0';
        cmd->salary = value;
    } else if (strcmp(cmdStr, "delete") == 0) {
        cmd->type = CMD_DELETE;
        strncpy(cmd->name, nameBuffer, sizeof(cmd->name) - 1);
        cmd->name[sizeof(cmd->name) - 1] = '\0';
    } else if (strcmp(cmdStr, "search") == 0) {
        cmd->type = CMD_SEARCH;
        strncpy(cmd->name, nameBuffer, sizeof(cmd->name) - 1);
        cmd->name[sizeof(cmd->name) - 1] = '\0';
    } else if (strcmp(cmdStr, "print") == 0) {
        cmd->type = CMD_PRINT;
    }
}

// Process a command in a thread
void *processCommand(void *arg) {
    ThreadArgs *threadArgs = (ThreadArgs *)arg;
    Command cmd = threadArgs->cmd;
    FILE *outputFile = threadArgs->outputFile;
    
    switch (cmd.type) {
        case CMD_INSERT:
            insertRecord(cmd.name, cmd.salary, outputFile);
            break;
        case CMD_DELETE:
            deleteRecord(cmd.name, outputFile);
            break;
        case CMD_SEARCH:
            searchRecord(cmd.name, outputFile);
            break;
        case CMD_PRINT:
            printHashTable(outputFile);
            break;
    }
    
    return NULL;
}
