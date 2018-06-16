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
    cache = new ReaderWriterCache(1,120);
}

void RequestDispatcherThread::run()
{
    forever
    {
        if (hasDebugLog)
            qDebug() << "Waiting for request...";
        Request req = requests->get();

        Option<Response> cacheResponse = cache->tryGetCachedResponse(req);

        // if the reponse is in the cache, put it in buffer
        if(cacheResponse.hasValue()){
            responses->put(cacheResponse.value());
        } else {
            // else start a new request processor
            RequestProcessor* reqProcesser = new RequestProcessor(req, responses,
                                                                  hasDebugLog, cache);
            threadList.push_back(reqProcesser);
            reqProcesser->start();
        }

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
