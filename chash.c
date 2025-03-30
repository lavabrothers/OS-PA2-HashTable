/* chash.c - Main entry point */
#include "hash_table.h"
#include "parser.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define COMMAND_FILE "commands.txt"
#define OUTPUT_FILE "output.txt"

pthread_rwlock_t rwlock;

int main() {
    FILE *cmd_file = fopen(COMMAND_FILE, "r");
    if (!cmd_file) {
        perror("Error opening commands.txt");
        return 1;
    }

    FILE *out_file = fopen(OUTPUT_FILE, "w");
    if (!out_file) {
        perror("Error opening output.txt");
        fclose(cmd_file);
        return 1;
    }

    pthread_rwlock_init(&rwlock, NULL);
    
    parse_commands(cmd_file, out_file);
    
    pthread_rwlock_destroy(&rwlock);
    fclose(cmd_file);
    fclose(out_file);
    
    return 0;
}