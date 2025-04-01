#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>
#include <stdint.h>

typedef struct hash_struct {
    uint32_t hash;
    char name[50];
    uint32_t salary;
    struct hash_struct *next;
} hashRecord;

// Hash table operations
void initHashTable();
void destroyHashTable();
int insertRecord(const char *name, uint32_t salary, FILE *outputFile);
int deleteRecord(const char *name, FILE *outputFile);
uint32_t searchRecord(const char *name, FILE *outputFile);
void printHashTable(FILE *outputFile);

// Jenkins one-at-a-time hash function
uint32_t hashFunction(const char *key);

#endif // HASH_TABLE_H
