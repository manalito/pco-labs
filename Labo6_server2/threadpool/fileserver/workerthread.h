#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include "runnable.h"

/*
 *
 */
class WorkerThread : public QThread
{
public:
    // constructor
    WorkerThread(Runnable*);
    WorkerThread();
    // run function inherited from QThread
    void run();
    // set a different runnable as attribute
    void setRunnable(Runnable*);

private :
    // a Runnable whose run function we will execute in the thread's run function
    Runnable* handler;
};

#endif // WORKERTHREAD_H
