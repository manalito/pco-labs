#include "workerthread.h"

WorkerThread::WorkerThread(){}
WorkerThread::WorkerThread(Runnable* handler): handler(handler){}

void WorkerThread::run(){
    handler->run();
}

void WorkerThread::setRunnable(Runnable* handler){
    this->handler = handler;
}
