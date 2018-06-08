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

/*
 * This class listen to requests and dispatch them to a threadpool
 */
class RequestDispatcherThread : public QThread
{

public:
    // constructor
    RequestDispatcherThread(AbstractBuffer<Request>* requests
                            ,AbstractBuffer<Response>* responses,
                            bool hasDebugLog);
    /*
     * The run function. When an element is added to the requests buffer,
     * then creates a RequestHandler to process the request and pass it to the
     * threadpool.
     */
    void run();

    //destructor
    ~RequestDispatcherThread();

private:

    // the request buffer
    AbstractBuffer<Request>* requests;
    // the responses buffer where RequestHandlers will put their responses
    AbstractBuffer<Response>* responses;
    // will display debug messages if true
    bool hasDebugLog;
    // a threadpool that will handle the requests
    ThreadPool* threadPool;

};
#endif // REQUESTDISPATCHERTHREAD_H
