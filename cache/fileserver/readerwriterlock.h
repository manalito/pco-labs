#ifndef READERWRITERLOCK_H
#define READERWRITERLOCK_H
#include <QSemaphore>
#include "hoaremonitor.h"

class ReaderWriterLock : public HoareMonitor
{

public:
    // a condition to manage a writer's permission to write
    Condition writerCond;

    // number of readers currently reading
    int nbReaders;

    ReaderWriterLock();

    // when a reader start reading
    void lockReading();
    // when a writer start writing
    void lockWriting();
    // when a reader stop reading
    void unlockReading();
    // when a writer stop writing
    void unlockWriting();
};

#endif // READERWRITERLOCK_H
