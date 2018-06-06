#include "requestdispatcherthread.h"
#include <QList>

RequestDispatcherThread::RequestDispatcherThread(AbstractBuffer<Request>* requests,
                                                 AbstractBuffer<Response>* responses,
                                                 bool hasDebugLog)
    : hasDebugLog(hasDebugLog),responses(responses),requests(requests)
{
    if (hasDebugLog)
        qDebug() << "Launching request dispatcher";
    threadPool = new ThreadPool(2);
}

void RequestDispatcherThread::run()
{
    forever
    {
        if (hasDebugLog)
            qDebug() << "Waiting for request...";
        Request req = requests->get();

        // create a runnable to handle request
        RequestHandler requestHandler = RequestHandler(req, condition, mutex,
                                                       responses, hasDebugLog);

        // pass the runnable to the thread pool
        threadPool->start(&requestHandler);

    }
}
RequestDispatcherThread::~RequestDispatcherThread(){
    delete condition;
    delete mutex;
    delete threadPool;
}
