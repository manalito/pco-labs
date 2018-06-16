#include "readerwriterlock.h"
#include <QDebug>

ReaderWriterLock::ReaderWriterLock() : nbReaders(0)
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
        // the last reader signals to a waiting writer that he can now write
        signal(writerCond);
    monitorOut();
}

void ReaderWriterLock::lockWriting() {
    monitorIn();
    if(nbReaders > 0){
        // wait for the last reader to finish
        wait(writerCond);
    }
}

void ReaderWriterLock::unlockWriting() {
    signal(writerCond);
    monitorOut();
}
