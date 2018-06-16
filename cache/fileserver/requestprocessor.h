#ifndef REQUESTPROCESSOR_H
#define REQUESTPROCESSOR_H

#include <QObject>
#include <QThread>
#include "request.h"
#include "response.h"
#include "abstractbuffer.h"

class RequestProcessor:public QThread
{

public:
    RequestProcessor(Request request, AbstractBuffer<Response>* responses,
                   bool hasDebugLog);
    void run();

private:
    Request request;
    AbstractBuffer<Response>* responses;
    bool hasDebugLog;
};

#endif // REQUESTPROCESSOR_H
