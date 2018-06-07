#include <QDebug>
#include "requesthandler.h"
#include "filereader.h"

RequestHandler::RequestHandler(Request request, QWaitCondition* condition,
                               QMutex* mutex, AbstractBuffer<Response>* responses,
                               bool hasDebugLog): request(request),
                               condition(condition), mutex(mutex),
                               responses(responses), hasDebugLog(hasDebugLog){
    ++counter;
    name = baseName + counter;
}

RequestHandler::RequestHandler(const RequestHandler &handler){
    request = handler.request;
    condition = handler.condition;
    mutex = handler.mutex;
    hasDebugLog = handler.hasDebugLog;
}

void RequestHandler::handle()
{
    FileReader reader(request.getFilePath(), hasDebugLog);
    // if the path corresponds to a file, read it and put the result in response
    if (reader.fileExists()) {
        Response response(request, reader.readAll());
        responses->put(response);
    } else {
        Response response(request, "File not found!");
        responses->put(response);
    }
}

void RequestHandler::run(){
    handle();
    mutex->lock();
    condition->wakeOne();
    mutex->unlock();
}

QString RequestHandler::id(){
    return name;
}

RequestHandler::~RequestHandler(){
}

int RequestHandler::counter = 0;
QString RequestHandler::baseName = "handler";
