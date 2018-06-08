#include "workerthread.h"
#include <QDebug>

WorkerThread::WorkerThread(Runnable* handler, QWaitCondition* condition,
                           QMutex* mutex): handler(handler), condition(condition),
                           mutex(mutex){}

void WorkerThread::run(){
    handler->run();
    condition->wakeOne();
}

void WorkerThread::setRunnable(Runnable* handler){
    this->handler = handler;
}

bool WorkerThread::tryDeleteRunnable(){
    if(this->isFinished()){
        delete handler;
        return true;
    }
    return false;
}
