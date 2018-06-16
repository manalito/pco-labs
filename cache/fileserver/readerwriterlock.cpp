#include "readerwriterlock.h"

ReaderWriterLock::ReaderWriterLock()
 {}

void ReaderWriterLock::lockReading() {
    monitorIn();
    nbReaders++;
    monitorOut();
}

void ReaderWriterLock::unlockReading() {
    monitorIn();
    nbReaders -=1;
    if(nbReaders==0)
        signal(writerCond);
    monitorOut();
}

void ReaderWriterLock::lockWriting() {
    monitorIn();
    wait(writerCond);
}

void ReaderWriterLock::unlockWriting() {
    signal(writerCond);
    monitorOut();
}
