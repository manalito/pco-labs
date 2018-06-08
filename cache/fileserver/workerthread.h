#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <QWaitCondition>
#include <QMutex>
#include "runnable.h"

// A thread class used in a threadpool to process a runnable
class WorkerThread : public QThread
{
public:
    // constructors
    WorkerThread(Runnable*, QWaitCondition*, QMutex*);
    // run function inherited from QThread
    void run();
    // set a different runnable as attribute
    void setRunnable(Runnable*);
    // try to delete the runnable, return true if success or false if the
    // thread is not finished
    bool tryDeleteRunnable();

private :
    // runnable whose run function will be runned in the thread's run function
    Runnable* handler;
    // condition to manage the threads in the pool
    QWaitCondition* condition;
    // mutex used in the condition
    QMutex* mutex;

};

#endif // WORKERTHREAD_H
