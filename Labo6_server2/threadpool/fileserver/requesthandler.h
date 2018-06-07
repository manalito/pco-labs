#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H
#include <QWaitCondition>
#include <QMutex>
#include "request.h"
#include "response.h"
#include "runnable.h"
#include "abstractbuffer.h"

class RequestHandler : public Runnable
{
private:
    Request request;
    QWaitCondition* condition;
    QMutex* mutex;
    AbstractBuffer<Response>* responses;
    bool hasDebugLog;
    static int counter;
    static QString baseName;
    QString name;

public:
    RequestHandler(Request request, QWaitCondition* condition,
                   QMutex* mutex, AbstractBuffer<Response>* responses,
                   bool hasDebugLog);
    RequestHandler(const RequestHandler& handler);
    void handle();
    void run();
    QString id();
    ~RequestHandler();

};

#endif // REQUESTHANDLER_H
