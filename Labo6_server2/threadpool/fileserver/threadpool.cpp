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
        qDebug() << "creating new thread" << endl;
        worker = new WorkerThread(runnable, condition, mutex);
        threadList.push_back(worker);

    // otherwise, take a free thread in the pool or wait until one is available
    } else {
        if((worker = freeThread()) == nullptr){
            qDebug() << "retrieving old thread" << endl;
            mutex->lock();
            // wait until a thread from the pool has finished its task
            condition->wait(mutex);
            mutex->unlock();

            // "while" is necessary if the thread takes some time to properly finish
            while((worker = freeThread()) == nullptr){
                qDebug() << "attente active" << endl;
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


