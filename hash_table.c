#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hash_table.h"
#include "rw_lock.h"

// Global hash table head
static hashRecord *hashTableHead = NULL;

// Initialize hash table
void initHashTable() {
    hashTableHead = NULL;
}

// Destroy hash table and free memory
void destroyHashTable() {
    hashRecord *current = hashTableHead;
    while (current != NULL) {
        hashRecord *temp = current;
        current = current->next;
        free(temp);
    }
    hashTableHead = NULL;
}

// Jenkins one-at-a-time hash function
uint32_t hashFunction(const char *key) {
    uint32_t hash = 0;
    size_t i = 0;
    
    while (key[i] != '\0') {
        hash += key[i++];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    
    return hash;
}

// Insert a record into the hash table
int insertRecord(const char *name, uint32_t salary, FILE *outputFile) {
    uint32_t hash = hashFunction(name);
    
    // Get current timestamp
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    time_t timestamp = ts.tv_sec;
    
    // Log the operation
    fprintf(outputFile, "%ld,INSERT,%s,%u\n", timestamp, name, salary);
    
    // Acquire write lock
    acquireWriteLock(outputFile);
    
    // Check if the record already exists
    hashRecord *current = hashTableHead;
    hashRecord *prev = NULL;
    
    while (current != NULL && current->hash < hash) {
        prev = current;
        current = current->next;
    }
    
    // If found, update the salary
    if (current != NULL && current->hash == hash && strcmp(current->name, name) == 0) {
        current->salary = salary;
        releaseWriteLock(outputFile);
        return 1;
    }
    
    // Otherwise, create a new record
    hashRecord *newRecord = (hashRecord *)malloc(sizeof(hashRecord));
    if (!newRecord) {
        releaseWriteLock(outputFile);
        return -1;
    }
    
    newRecord->hash = hash;
    strncpy(newRecord->name, name, sizeof(newRecord->name) - 1);
    newRecord->name[sizeof(newRecord->name) - 1] = '\0';
    newRecord->salary = salary;
    
    // Insert the new record in sorted order by hash
    if (prev == NULL) {
        // Insert at the beginning
        newRecord->next = hashTableHead;
        hashTableHead = newRecord;
    } else {
        // Insert in the middle or at the end
        newRecord->next = prev->next;
        prev->next = newRecord;
    }
    
    releaseWriteLock(outputFile);
    return 0;
}

// Delete a record from the hash table
int deleteRecord(const char *name, FILE *outputFile) {
    uint32_t hash = hashFunction(name);
    
    // Get current timestamp
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    time_t timestamp = ts.tv_sec;
    
    // Log the operation
    fprintf(outputFile, "%ld,DELETE,%s\n", timestamp, name);
    
    // Wait for all insert operations to complete
    waitForInserts(outputFile);
    
    // Acquire write lock
    acquireWriteLock(outputFile);
    
    // Search for the record
    hashRecord *current = hashTableHead;
    hashRecord *prev = NULL;
    
    while (current != NULL && (current->hash < hash || 
           (current->hash == hash && strcmp(current->name, name) != 0))) {
        prev = current;
        current = current->next;
    }
    
    // If found, remove it
    if (current != NULL && current->hash == hash && strcmp(current->name, name) == 0) {
        if (prev == NULL) {
            // It's the first node
            hashTableHead = current->next;
        } else {
            // It's in the middle or at the end
            prev->next = current->next;
        }
        
        free(current);
        releaseWriteLock(outputFile);
        return 1;
    }
    
    releaseWriteLock(outputFile);
    return 0;
}

// Search for a record in the hash table
uint32_t searchRecord(const char *name, FILE *outputFile) {
    uint32_t hash = hashFunction(name);
    
    // Get current timestamp
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    time_t timestamp = ts.tv_sec;
    
    // Log the operation
    fprintf(outputFile, "%ld,SEARCH,%s\n", timestamp, name);
    
    // Acquire read lock
    acquireReadLock(outputFile);
    
    // Search for the record
    hashRecord *current = hashTableHead;
    
    while (current != NULL && current->hash < hash) {
        current = current->next;
    }
    
    // If found, return the salary
    if (current != NULL && current->hash == hash && strcmp(current->name, name) == 0) {
        uint32_t result = current->salary;
        releaseReadLock(outputFile);
        
        // Print the record
        fprintf(outputFile, "%u,%s,%u\n", hash, name, result);
        return result;
    }
    
    releaseReadLock(outputFile);
    
    // Record not found
    fprintf(outputFile, "No Record Found\n");
    return 0;
}

// Print the entire hash table
void printHashTable(FILE *outputFile) {
    // Acquire read lock
    acquireReadLock(outputFile);
    
    // Print all records
    hashRecord *current = hashTableHead;
    
    while (current != NULL) {
        fprintf(outputFile, "%u,%s,%u\n", current->hash, current->name, current->salary);
        current = current->next;
    }
    
    releaseReadLock(outputFile);
}