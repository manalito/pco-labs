#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H
#include <QWaitCondition>
#include <QMutex>
#include "request.h"
#include "response.h"
#include "runnable.h"

class RequestHandler : public Runnable
{
private:
    Request request;
    bool hasDebugLog;
    static int counter;
    static QString baseName;
    QString name;
    QWaitCondition* condition;
    QMutex* mutex;

public:
    RequestHandler(Request request, QWaitCondition* condition,
                   QMutex* mutex, bool hasDebugLog);
    RequestHandler(const RequestHandler& handler);
    Response handle();
    void run();
    QString id();
    ~RequestHandler();

};

#endif // REQUESTHANDLER_H
