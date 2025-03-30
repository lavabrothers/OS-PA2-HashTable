/* hash_table.h - Header for Hash Table */
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdint.h>
#include <pthread.h>

typedef struct hash_struct {
    uint32_t hash;
    char name[50];
    uint32_t salary;
    struct hash_struct *next;
} hashRecord;

void insert(const char *name, uint32_t salary, FILE *out_file);
void delete(const char *name, FILE *out_file);
void search(const char *name, FILE *out_file);
void print_table(FILE *out_file);
uint32_t jenkins_one_at_a_time_hash(const char *key);

#endif /* HASH_TABLE_H */