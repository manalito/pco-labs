#include "requestdispatcherthread.h"
#include "requestprocessor.h"
#include <QList>

RequestDispatcherThread::RequestDispatcherThread(AbstractBuffer<Request>* requests,
                                                 AbstractBuffer<Response>* responses,
                                                 bool hasDebugLog)
    : hasDebugLog(hasDebugLog),responses(responses),requests(requests)
{
    if (hasDebugLog)
        qDebug() << "Launching request dispatcher";
}

void RequestDispatcherThread::run()
{
    forever
    {
        if (hasDebugLog)
            qDebug() << "Waiting for request...";
        Request req = requests->get();

        // start a new request processor
        RequestProcessor* reqProcesser = new RequestProcessor(req, responses,
                                                              hasDebugLog);
        threadList.push_back(reqProcesser);
        reqProcesser->start();

        // clean the thread list
        for(RequestProcessor* req : threadList)
        {
            if(req->isFinished())
            {
               threadList.removeOne(req);
               delete(req);
            }
        }
    }
}
