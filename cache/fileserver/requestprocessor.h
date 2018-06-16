#ifndef REQUESTPROCESSOR_H
#define REQUESTPROCESSOR_H

#include <QObject>
#include <QThread>
#include "request.h"
#include "response.h"
#include "abstractbuffer.h"
#include "option.h"
#include "readerwritercache.h"

/* this class processes a request and put the corresponding response in a buffer
 * and in a cache
 */
class RequestProcessor:public QThread
{

public:
    // constructor
    RequestProcessor(Request request, AbstractBuffer<Response>* responses,
                   bool hasDebugLog, ReaderWriterCache* cache);
    // run function inherited from QThread
    void run();

private:
    // request to process
    Request request;
    // buffer to put the response to a request
    AbstractBuffer<Response>* responses;
    // boolean to print debug logs
    bool hasDebugLog;
    // cache to put the response to a request
    ReaderWriterCache* cache;
};

#endif // REQUESTPROCESSOR_H
