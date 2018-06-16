#ifndef READERWRITERLOCK_H
#define READERWRITERLOCK_H
#include <QSemaphore>
#include "hoaremonitor.h"

class ReaderWriterLock : public HoareMonitor
{

public:
    Condition writerCond;
    Condition readerCond;
    int nbReaders, nbWriters;

    ReaderWriterLock();

    void lockReading();
    void lockWriting();
    void unlockReading();
    void unlockWriting();
};

#endif // READERWRITERLOCK_H
