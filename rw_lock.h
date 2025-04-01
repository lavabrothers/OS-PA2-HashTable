#ifndef RW_LOCK_H
#define RW_LOCK_H

#include <stdio.h>

// Reader-writer lock operations
void initRWLock();
void destroyRWLock();
void acquireReadLock(FILE *outputFile);
void releaseReadLock(FILE *outputFile);
void acquireWriteLock(FILE *outputFile);
void releaseWriteLock(FILE *outputFile);
void waitForInserts(FILE *outputFile);
void signalInsertsDone();
int getLockAcquisitionCount();
int getLockReleaseCount();

#endif // RW_LOCK_H