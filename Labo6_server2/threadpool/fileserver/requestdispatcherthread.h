#ifndef REQUESTDISPATCHERTHREAD_H
#define REQUESTDISPATCHERTHREAD_H

#include <QDebug>
#include <QThread>
#include <QList>
#include <QWaitCondition>
#include "abstractbuffer.h"
#include "threadpool.h"
#include "response.h"
#include "request.h"
#include "requesthandler.h"

class RequestDispatcherThread : public QThread
{

public:
    RequestDispatcherThread(AbstractBuffer<Request>* requests
                            ,AbstractBuffer<Response>* responses,
                            bool hasDebugLog);
    void run();
    ~RequestDispatcherThread();

private:

    AbstractBuffer<Request>* requests;
    AbstractBuffer<Response>* responses;
    bool hasDebugLog;
    ThreadPool* threadPool;
    QWaitCondition* condition = new QWaitCondition();
    QMutex* mutex = new QMutex();

};
#endif // REQUESTDISPATCHERTHREAD_H
