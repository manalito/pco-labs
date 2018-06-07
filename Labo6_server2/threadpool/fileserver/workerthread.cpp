#include "workerthread.h"
#include <QDebug>

WorkerThread::WorkerThread(){}
WorkerThread::WorkerThread(Runnable* handler, QWaitCondition* condition,
                           QMutex* mutex): handler(handler), condition(condition),
                           mutex(mutex){}

void WorkerThread::run(){
    handler->run();
    qInfo() << "run1" << endl;
    condition->wakeOne();
}

void WorkerThread::setRunnable(Runnable* handler){
    this->handler = handler;
}
