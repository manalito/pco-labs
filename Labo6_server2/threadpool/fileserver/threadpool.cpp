#include "threadpool.h"

/* Start a runnable. If a thread in the pool is avaible, assign the
 &
→
runnable to it. If no thread is available but the pool can grow, &
→create a new pool thread and assign the runnable to it. If no &
→thread is available and the pool is at max capacity, block the &
→caller until a thread becomes available again. */

ThreadPool::ThreadPool(int maxThreadCount) :
    maxThreadCount(maxThreadCount){
    condition = new QWaitCondition();
    mutex = new QMutex();
}

void ThreadPool::start(Runnable *runnable) {

    WorkerThread* worker;
    mutex->lock();
    // if the pool is not full yet, create a new thread
    if(threadList.size() <= maxThreadCount){
        worker = new WorkerThread();
    } else {
        while(!(worker = freeThread())){
            condition->wait(mutex);
        }
    }
    mutex->unlock();
    worker->setRunnable(runnable);
    worker->start();
}

WorkerThread* ThreadPool::freeThread(){
    for(int i = 0; i < threadList.size(); ++i){
        if(threadList.at(i).isFinished()){
            return (WorkerThread*)&threadList.at(i);
        }
    }
    return nullptr;
}


