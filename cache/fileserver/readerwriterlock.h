#ifndef READERWRITERLOCK_H
#define READERWRITERLOCK_H

#include <hoaremonitor.h>


class ReaderWriterLock : public HoareMonitor
{

public:
    ReaderWriterLock();

    void lockReading();
    void lockWriting();
    void unlockReading();
    void unlockWriting();
};

#endif // READERWRITERLOCK_H
