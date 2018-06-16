#ifndef REQUESTDISPATCHERTHREAD_H
#define REQUESTDISPATCHERTHREAD_H

#include <QDebug>
#include <QThread>
#include <QList>
#include "abstractbuffer.h"
#include "requestprocessor.h"

class RequestDispatcherThread : public QThread
{

public:
    RequestDispatcherThread(AbstractBuffer<Request>* requests
                            ,AbstractBuffer<Response>* responses,
                            bool hasDebugLog);
    void run();

private:

    AbstractBuffer<Request>* requests;
    AbstractBuffer<Response>* responses;
    bool hasDebugLog;
    QList<RequestProcessor*> threadList;

    ReaderWriterCache* cache;
};
#endif // REQUESTDISPATCHERTHREAD_H
