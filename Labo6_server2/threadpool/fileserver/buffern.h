#ifndef BUFFERN_H
#define BUFFERN_H

#include "abstractbuffer.h"
#include <QSemaphore>

const int NoInitTamponN = 10;

/*
 * A container class to simulate a buffer protected against concurrent accesses
 */
template<typename T> class BufferN : public AbstractBuffer<T> {
protected:
    // the "buffer", an array of elements of type T
    T *elements;

    // indices of reader and writer pointers in elements array
    int writePointer, readPointer;

    // current number of elements in elements array
    int nbElements;

    // size of elements array
    int bufferSize;

    // will display debug messages if true
    bool debug;

    // semaphores to protect concurrent accesses
    QSemaphore mutex, waitProd, waitConso;

    // counters of waiting producers and consumers
    unsigned nbWaitingProd, nbWaitingConso;

public:

    // constructor
    BufferN(unsigned int size, bool debug) : debug(debug), mutex(1) {
        if ((elements = new T[size]) != 0) {
            writePointer = readPointer = nbElements = 0;
            nbWaitingProd = nbWaitingConso = 0;
            bufferSize = size;
            return;
        }
        // Exception
        throw NoInitTamponN;
    }

    // destructor
    virtual ~BufferN() {}

    /*
     * Try to put an item in the buffer.
     * If the buffer is full, then do not put the item and return false.
     * If the buffer is not full, put the item and return true.
     */
    virtual bool tryPut(T item){
        mutex.acquire();
        if(nbElements == bufferSize){
            mutex.release();
            qInfo() << "bufferN full, could not put element" << endl;
            return false;
        }
        put(item);
        return true;
    }

    // Return the "oldest" item in the buffer
    virtual T get(void) {
        T item;
        mutex.acquire();
        if (nbElements == 0) {
            nbWaitingConso += 1;
            mutex.release();
            waitConso.acquire();
        }
        item = elements[readPointer];
        readPointer = (readPointer + 1)
                      % bufferSize;
        nbElements --;
        mutex.release();

        return item;
    }

private :
    // put an item in the buffer
    virtual void put(T item) {

        elements[writePointer] = item;
        writePointer = (writePointer + 1)
                       % bufferSize;
        nbElements ++;
        if (nbWaitingConso > 0) {
            nbWaitingConso -= 1;
            waitConso.release();
        }
        else {
            mutex.release();
        }
    }
};

#endif // BUFFERN_H
