#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "rw_lock.h"

// Reader-writer lock
static pthread_rwlock_t rwlock;

// Condition variable for delete operations
static pthread_mutex_t deleteMutex;
static pthread_cond_t deleteCond;
static int insertCount = 0;
static int deleteWaiting = 0;

// Lock operation counters
static int lockAcquisitionCount = 0;
static int lockReleaseCount = 0;

// Initialize reader-writer lock
void initRWLock() {
    pthread_rwlock_init(&rwlock, NULL);
    pthread_mutex_init(&deleteMutex, NULL);
    pthread_cond_init(&deleteCond, NULL);
    insertCount = 0;
    deleteWaiting = 0;
    lockAcquisitionCount = 0;
    lockReleaseCount = 0;
}

// Destroy reader-writer lock
void destroyRWLock() {
    pthread_rwlock_destroy(&rwlock);
    pthread_mutex_destroy(&deleteMutex);
    pthread_cond_destroy(&deleteCond);
}

// Acquire read lock
void acquireReadLock(FILE *outputFile) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    time_t timestamp = ts.tv_sec;
    
    fprintf(outputFile, "%ld,READ LOCK ACQUIRED\n", timestamp);
    pthread_rwlock_rdlock(&rwlock);
    lockAcquisitionCount++;
}

// Release read lock
void releaseReadLock(FILE *outputFile) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    time_t timestamp = ts.tv_sec;
    
    pthread_rwlock_unlock(&rwlock);
    fprintf(outputFile, "%ld,READ LOCK RELEASED\n", timestamp);
    lockReleaseCount++;
}

// Acquire write lock
void acquireWriteLock(FILE *outputFile) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    time_t timestamp = ts.tv_sec;
    
    fprintf(outputFile, "%ld,WRITE LOCK ACQUIRED\n", timestamp);
    pthread_rwlock_wrlock(&rwlock);
    lockAcquisitionCount++;
    
    // For insert operations
    pthread_mutex_lock(&deleteMutex);
    insertCount++;
    pthread_mutex_unlock(&deleteMutex);
}

// Release write lock
void releaseWriteLock(FILE *outputFile) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    time_t timestamp = ts.tv_sec;
    
    pthread_rwlock_unlock(&rwlock);
    fprintf(outputFile, "%ld,WRITE LOCK RELEASED\n", timestamp);
    lockReleaseCount++;
    
    // Signal delete operations if an insert operation is completed
    pthread_mutex_lock(&deleteMutex);
    insertCount--;
    if (insertCount == 0 && deleteWaiting > 0) {
        pthread_cond_broadcast(&deleteCond);
    }
    pthread_mutex_unlock(&deleteMutex);
}

// Wait for all insert operations to complete before proceeding with delete
void waitForInserts(FILE *outputFile) {
    pthread_mutex_lock(&deleteMutex);
    
    if (insertCount > 0) {
        deleteWaiting++;
        
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        long timestamp = ts.tv_sec * 1000000000L + ts.tv_nsec;
        
        fprintf(outputFile, "%ld: WAITING ON INSERTS\n", timestamp);
        pthread_cond_wait(&deleteCond, &deleteMutex);
        
        clock_gettime(CLOCK_REALTIME, &ts);
        timestamp = ts.tv_sec * 1000000000L + ts.tv_nsec;
        
        fprintf(outputFile, "%ld: DELETE AWAKENED\n", timestamp);
        deleteWaiting--;
    }
    
    pthread_mutex_unlock(&deleteMutex);
}

// Get lock acquisition count
int getLockAcquisitionCount() {
    return lockAcquisitionCount;
}

// Get lock release count
int getLockReleaseCount() {
    return lockReleaseCount;
}
