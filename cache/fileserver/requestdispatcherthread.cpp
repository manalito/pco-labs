#include "requestdispatcherthread.h"
#include "requestprocessor.h"
#include <QList>
#include <QDebug>

RequestDispatcherThread::RequestDispatcherThread(AbstractBuffer<Request>* requests,
                                                 AbstractBuffer<Response>* responses,
                                                 bool hasDebugLog)
    : requests(requests), responses(responses), hasDebugLog(hasDebugLog)
{
    if (hasDebugLog)
        qDebug() << "Launching request dispatcher";
    cache = new ReaderWriterCache(1,120);
}

void RequestDispatcherThread::run()
{
    forever
    {
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
