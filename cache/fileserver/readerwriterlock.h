#ifndef READERWRITERLOCK_H
#define READERWRITERLOCK_H

#include "hoaremonitor.h"


class ReaderWriterLock : public HoareMonitor
{

public:
    QSemaphore mutexReaders;
    QSemaphore mutexWriters;
    QSemaphore writer;
    QSemaphore reader;
    QSemaphore mutex;
    int nbReaders, nbWriters;

    ReaderWriterLock();

    void lockReading();
    void lockWriting();
    void unlockReading();
    void unlockWriting();
};

#endif // READERWRITERLOCK_H
