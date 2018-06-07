#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <QWaitCondition>
#include <QMutex>
#include "runnable.h"

/*
 *
 */
class WorkerThread : public QThread
{
public:
    // constructor
    WorkerThread(Runnable*, QWaitCondition*, QMutex*);
    WorkerThread();
    // run function inherited from QThread
    void run();
    // set a different runnable as attribute
    void setRunnable(Runnable*);

private :
    // a Runnable whose run function we will execute in the thread's run function
    Runnable* handler;
    QWaitCondition* condition;
    QMutex* mutex;

};

#endif // WORKERTHREAD_H
