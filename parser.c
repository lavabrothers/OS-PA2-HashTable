/* parser.c - Implementation of Command Parsing */
#include "parser.h"
#include "hash_table.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void execute_command(const char *command, FILE *out_file) {
    // Example: Write the command to the output file
    fprintf(out_file, "Executing command: %s\n", command);
}

void parse_commands(FILE *cmd_file, FILE *out_file) {
    char command[256];
    while (fgets(command, sizeof(command), cmd_file)) {
        execute_command(command, out_file);
    }
}
