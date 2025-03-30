/* parser.c - Implementation of Command Parsing */
#include "parser.h"
#include "hash_table.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

void *execute_command(void *arg) {
    char *command = (char *)arg;
    char action[10], name[50];
    uint32_t salary;
    
    sscanf(command, "%[^,],%[^,],%u", action, name, &salary);
    
    if (strcmp(action, "insert") == 0) insert(name, salary, stdout);
    else if (strcmp(action, "delete") == 0) delete_record(name, stdout);
    else if (strcmp(action, "search") == 0) search(name, stdout);
    else if (strcmp(action, "print") == 0) print_table(stdout);
    
    free(arg);
    return NULL;
}

void parse_commands(FILE *cmd_file) 

    {
    char line[128];
    while (fgets(line, sizeof(line), cmd_file)) {
        char *cmd = strdup(line);
        pthread_t thread;
        pthread_create(&thread, NULL, execute_command, cmd);
        pthread_detach(thread);
    }
}
