#ifndef REQUESTPROCESSOR_H
#define REQUESTPROCESSOR_H

#include <QObject>
#include <QThread>
#include "request.h"
#include "response.h"
#include "abstractbuffer.h"
#include "option.h"
#include "readerwritercache.h"

class RequestProcessor:public QThread
{

public:
    RequestProcessor(Request request, AbstractBuffer<Response>* responses,
                   bool hasDebugLog, ReaderWriterCache* cache);
    void run();

private:
    Request request;
    AbstractBuffer<Response>* responses;
    bool hasDebugLog;
    ReaderWriterCache* cache;
};

#endif // REQUESTPROCESSOR_H
