#ifndef REQUESTDISPATCHERTHREAD_H
#define REQUESTDISPATCHERTHREAD_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include "abstractbuffer.h"
#include "request.h"


class RequestDispatcherThread: public QThread {
    Q_OBJECT

public:
    RequestDispatcherThread(AbstractBuffer<Request>* requests, bool hasDebugLog): requests(requests), hasDebugLog(hasDebugLog) {
        if (hasDebugLog)
            qDebug() << "Created request dispatcher thread";
        qRegisterMetaType<Request>("Request");
    }

protected:
    void run();

private:
    AbstractBuffer<Request>* requests;
    bool hasDebugLog;
};

#endif // REQUESTDISPATCHERTHREAD_H
