#include "threadpool.h"

ThreadPool::ThreadPool(int maxThreadCount) :
    maxThreadCount(maxThreadCount){
    condition = new QWaitCondition();
    mutex = new QMutex();
    threadList = QList<WorkerThread*>();
}

void ThreadPool::start(Runnable *runnable) {

    WorkerThread* worker;
    mutex->lock();
    // if the pool is not full yet, create a new thread
    if(threadList.size() < maxThreadCount){
        worker = new WorkerThread();
        threadList.push_back(worker);
    } else {
        qInfo() << "yup" << endl;
        while((worker = freeThread()) == nullptr){
            qInfo() << "im in " << endl;
            condition->wait(mutex);
            qInfo() << "im out" << endl;
        }
    }
    mutex->unlock();
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


