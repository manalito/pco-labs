#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <QDebug>
#include "runnable.h"
#include "requesthandler.h"
#include "workerthread.h"
#include <QList>
#include <QThread>
#include <QWaitCondition>
#include <QMutex>

/*
 * This class creates and maintains a WorkerThread pool of a given size. It can
 * receive runnables to be processed and pass them to an available WorkerThread
 * in the pool.
 */
class ThreadPool
{
public:

    // constructor
    ThreadPool(int maxThreadCount);

/* Start a runnable. If a thread in the pool is avaible, assign the
&runnable to it. If no thread is available but the pool can grow, &
create a new pool thread and assign the runnable to it. If no &
thread is available and the pool is at max capacity, block the &
caller until a thread becomes available again. */
void start(Runnable* runnable);

private:
// maximum number of theads in the pool
const int maxThreadCount;

// current number of theads in the pool
int currentThreadCount;

// WorkerThread list containing all the threads, available or not
QList<WorkerThread*> threadList;

// condition to manage the threads in the pool
QWaitCondition* condition;

// mutex used in the condition
QMutex* mutex;

/* return the first available WorkerThread from threadlist, return nullptr
 * if they are all busy
 */
WorkerThread* freeThread();
};

#endif // THREADPOOL_H
