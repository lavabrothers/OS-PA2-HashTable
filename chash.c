#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>
#include "hash_table.h"
#include "rw_lock.h"
#include "command_processor.h"

// Get current timestamp in nanoseconds
static long getNanosecondTimestamp() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec * 1000000000L + ts.tv_nsec;
}

int main() {
    FILE *cmdFile = fopen("commands.txt", "r");
    if (!cmdFile) {
        fprintf(stderr, "Error: Unable to open commands.txt\n");
        return 1;
    }

    // Create output file
    FILE *outputFile = fopen("output.txt", "w");
    if (!outputFile) {
        fprintf(stderr, "Error: Unable to create output.txt\n");
        fclose(cmdFile);
        return 1;
    }

    // Initialize hash table and locks
    initHashTable();
    initRWLock();

    // Process the first line to get number of threads
    char line[256];
    if (fgets(line, sizeof(line), cmdFile)) {
        char cmd[20];
        int numThreads, dummy;
        sscanf(line, "%[^,],%d,%d", cmd, &numThreads, &dummy);
        
        if (strcmp(cmd, "threads") == 0) {
            // Print thread count
            fprintf(outputFile, "Running %d threads\n", numThreads);
            
            // Create command array
            Command *commands = (Command *)malloc(numThreads * sizeof(Command));
            
            // Read and parse remaining commands
            int cmdCount = 0;
            while (fgets(line, sizeof(line), cmdFile) && cmdCount < numThreads) {
                parseCommand(line, &commands[cmdCount]);
                cmdCount++;
            }

            // Process commands in separate threads
            pthread_t *threads = (pthread_t *)malloc(numThreads * sizeof(pthread_t));
            ThreadArgs *args = (ThreadArgs *)malloc(numThreads * sizeof(ThreadArgs));
            
            for (int i = 0; i < numThreads; i++) {
                args[i].cmd = commands[i];
                args[i].outputFile = outputFile;
                pthread_create(&threads[i], NULL, processCommand, (void *)&args[i]);
            }
            
            // Wait for all threads to complete
            for (int i = 0; i < numThreads; i++) {
                pthread_join(threads[i], NULL);
            }
            
            // Print that all threads are finished
            fprintf(outputFile, "Finished all threads.\n");
            
            // Print final summary
            fprintf(outputFile, "Number of lock acquisitions:  %d\n", getLockAcquisitionCount());
            fprintf(outputFile, "Number of lock releases:  %d\n", getLockReleaseCount());
            
            // Print final hash table contents
            printHashTable(outputFile);
            
            // Clean up
            free(commands);
            free(threads);
            free(args);
        }
    }

    // Clean up resources
    destroyHashTable();
    destroyRWLock();
    fclose(cmdFile);
    fclose(outputFile);
    
    return 0;
}