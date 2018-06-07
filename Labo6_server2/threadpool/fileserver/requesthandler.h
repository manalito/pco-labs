#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H
#include <QWaitCondition>
#include <QMutex>
#include "request.h"
#include "response.h"
#include "runnable.h"
#include "abstractbuffer.h"

/*
 * A class to handle a given request and put the corresponding reponse in a buffer
 */
class RequestHandler : public Runnable
{
private:
    // a request to process
    Request request;

    // a buffer to put the response to the request
    AbstractBuffer<Response>* responses;

    // will display debug messages if true
    bool hasDebugLog;

    // global counter to create an ID
    static int counter;

    // global String to create an ID
    static QString baseName;

    // ID of the requesthandler
    QString name;

public:

    // main constructor, will initialize the ID
    RequestHandler(Request request, AbstractBuffer<Response>* responses,
                   bool hasDebugLog);

    // copy constructor
    RequestHandler(const RequestHandler& handler);

    /*
     * Create a reader to check the current path. If it corresponds to a file,
     * then read it and put the content in responses buffer. Otherwise put an
     * error message in responses buffer.
     */
    void handle();

    // handle the request by calling the handle function and when it is done,
    // try to wake one waiting thread through the condition.
    void run();

    // return the id of the current requesthandler
    QString id();

    // destructor
    ~RequestHandler();

};

#endif // REQUESTHANDLER_H
