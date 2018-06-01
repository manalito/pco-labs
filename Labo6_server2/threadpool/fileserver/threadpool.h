#ifndef THREADPOOL_H
#define THREADPOOL_H


class ThreadPool
{
public:
ThreadPool(int maxThreadCount);

/* Start a runnable. If a thread in the pool is avaible, assign the
 &
→
runnable to it. If no thread is available but the pool can grow, &
→create a new pool thread and assign the runnable to it. If no &
→thread is available and the pool is at max capacity, block the &
→caller until a thread becomes available again. */
void start(Runnable* runnable);
}

#endif // THREADPOOL_H
