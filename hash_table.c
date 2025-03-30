/* hash_table.c - Implementation of Hash Table */
#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static hashRecord *head = NULL;
extern pthread_rwlock_t rwlock;

uint32_t jenkins_one_at_a_time_hash(const char *key) {
    uint32_t hash = 0;
    while (*key) {
        hash += *key++;
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

void insert(const char *name, uint32_t salary, FILE *out_file) {
    uint32_t hash = jenkins_one_at_a_time_hash(name);
    pthread_rwlock_wrlock(&rwlock);
    fprintf(out_file, "WRITE LOCK ACQUIRED\n");

    hashRecord *curr = head;
    while (curr) {
        if (curr->hash == hash) {
            curr->salary = salary;
            pthread_rwlock_unlock(&rwlock);
            fprintf(out_file, "WRITE LOCK RELEASED\n");
            return;
        }
        curr = curr->next;
    }
    
    hashRecord *newNode = (hashRecord *)malloc(sizeof(hashRecord));
    newNode->hash = hash;
    strcpy(newNode->name, name);
    newNode->salary = salary;
    newNode->next = head;
    head = newNode;

    pthread_rwlock_unlock(&rwlock);
    fprintf(out_file, "WRITE LOCK RELEASED\n");
}

void delete_record(const char *name, FILE *out_file) {
    uint32_t hash = jenkins_one_at_a_time_hash(name);
    pthread_rwlock_wrlock(&rwlock);
    fprintf(out_file, "WRITE LOCK ACQUIRED\n");
    
    hashRecord *curr = head, *prev = NULL;
    while (curr) {
        if (curr->hash == hash) {
            if (prev) prev->next = curr->next;
            else head = curr->next;
            free(curr);
            break;
        }
        prev = curr;
        curr = curr->next;
    }
    
    pthread_rwlock_unlock(&rwlock);
    fprintf(out_file, "WRITE LOCK RELEASED\n");
}

void search(const char *name, FILE *out_file) {
    uint32_t hash = jenkins_one_at_a_time_hash(name);
    pthread_rwlock_rdlock(&rwlock);
    fprintf(out_file, "READ LOCK ACQUIRED\n");

    hashRecord *curr = head;
    while (curr) {
        if (curr->hash == hash) {
            fprintf(out_file, "%u,%s,%u\n", curr->hash, curr->name, curr->salary);
            pthread_rwlock_unlock(&rwlock);
            fprintf(out_file, "READ LOCK RELEASED\n");
            return;
        }
        curr = curr->next;
    }
    fprintf(out_file, "No Record Found\n");
    pthread_rwlock_unlock(&rwlock);
    fprintf(out_file, "READ LOCK RELEASED\n");
}

void print_table(FILE *out_file) {
    pthread_rwlock_rdlock(&rwlock);
    fprintf(out_file, "READ LOCK ACQUIRED\n");
    
    hashRecord *curr = head;
    while (curr) {
        fprintf(out_file, "%u,%s,%u\n", curr->hash, curr->name, curr->salary);
        curr = curr->next;
    }
    
    pthread_rwlock_unlock(&rwlock);
    fprintf(out_file, "READ LOCK RELEASED\n");
}