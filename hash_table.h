#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdint.h>
#include <pthread.h>
#include <stdio.h>  // Add this line

#define HASH_TABLE_SIZE 1024 // Example size, adjust as needed

// Define the structure of a hash table node
typedef struct HashNode {
    char *key;
    char *value;
    struct HashNode *next;
} HashNode;

// Declare the hash table
extern HashNode *hash_table[HASH_TABLE_SIZE];

typedef struct hash_struct {
    uint32_t hash;
    char name[50];
    uint32_t salary;
    struct hash_struct *next;
} hashRecord;

void insert(const char *name, uint32_t salary, FILE *out_file);
void delete_record(const char *name, FILE *out_file);
void search(const char *name, FILE *out_file);
void print_table(FILE *out_file);
uint32_t jenkins_one_at_a_time_hash(const char *key);
void write_hash_table_to_file(FILE *out_file);

#endif /* HASH_TABLE_H */
