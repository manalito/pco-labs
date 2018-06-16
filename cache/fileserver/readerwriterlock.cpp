#include "readerwriterlock.h"

ReaderWriterLock::ReaderWriterLock() :
    mutexReaders(1),
    mutexWriters(1),
    mutex(1),
    writer(1),
    reader(1),
    nbReaders(0),
    nbWriters(0) {}

void ReaderWriterLock::lockReading() {
mutexReaders.acquire();
reader.acquire();
mutex.acquire();

nbReaders++;
if(nbReaders ==1)
    writer.acquire();

mutex.release();
reader.release();
mutexReaders.release();
}

void ReaderWriterLock::unlockReading() {
mutex.acquire();
nbReaders -=1;
if(nbReaders==0)
    writer.release();
mutex.release();
}

void ReaderWriterLock::lockWriting() {
mutexWriters.acquire();
nbWriters++;
if(nbWriters==1)
    reader.acquire();
mutexWriters.release();
writer.acquire();
}

void ReaderWriterLock::unlockWriting() {
    writer.release();
    mutexWriters.acquire();
    nbWriters -=1;
    if(nbWriters==0)
        reader.release();
    mutexWriters.release();
}
