#ifndef REQUESTDISPATCHERTHREAD_H
#define REQUESTDISPATCHERTHREAD_H

#include "abstractbuffer.h"
#include "request.h"


class RequestDispatcherThread: public QThread {
    Q_OBJECT

public:
    RequestDispatcherThread(AbstractBuffer<Request>* requests, bool hasDebugLog): requests(requests), hasDebugLog(hasDebugLog) {
        if (hasDebugLog)
            qDebug() << "Created response dispatcher thread";
        qRegisterMetaType<Response>("Response");
    }

protected:
    void run();

private:
    AbstractBuffer<Request>* requests;
    bool hasDebugLog;

signals:
    void requestReady(Request request);
};

#endif // REQUESTDISPATCHERTHREAD_H
