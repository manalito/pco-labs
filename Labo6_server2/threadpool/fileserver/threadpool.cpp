#include "threadpool.h"

ThreadPool::ThreadPool(int maxThreadCount) :
    maxThreadCount(maxThreadCount){
    condition = new QWaitCondition();
    mutex = new QMutex();
    threadList = QList<WorkerThread*>();
}

void ThreadPool::start(Runnable *runnable) {

    WorkerThread* worker;
    // if the pool is not full yet, create a new thread
    if(threadList.size() < maxThreadCount){
        worker = new WorkerThread(runnable, condition, mutex);
        threadList.push_back(worker);
    } else {
        if((worker = freeThread()) == nullptr){
            mutex->lock();
            condition->wait(mutex);
            mutex->unlock();
            worker = freeThread();

            if(worker == nullptr){
                qInfo() << "error : a thread did not finish, exiting";
                exit(-1);
            }
        }
    }
    worker->setRunnable(runnable);
    worker->start();
}

WorkerThread* ThreadPool::freeThread(){
    for(int i = 0; i < threadList.size(); ++i){
        if(threadList.at(i)->isFinished()){
            return threadList.at(i);
        }
    }
    return nullptr;
}


