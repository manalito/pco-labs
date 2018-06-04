#include <QDebug>
#include "requesthandler.h"
#include "filereader.h"

RequestHandler::RequestHandler(Request request, QWaitCondition* condition,
                               QMutex* mutex, bool hasDebugLog): request(request),
                               condition(condition), mutex(mutex),
                               hasDebugLog(hasDebugLog){
    ++counter;
    name = baseName + counter;
}

RequestHandler::RequestHandler(const RequestHandler &handler){
    request = handler.request;
    condition = handler.condition;
    mutex = handler.mutex;
    hasDebugLog = handler.hasDebugLog;
}

Response RequestHandler::handle()
{
    if (hasDebugLog)
        qDebug() << "Handling request '" << this->request.getFilePath() << "'...";
    FileReader reader(this->request.getFilePath(), this->hasDebugLog);
    if (reader.fileExists()) {
        return Response(request, reader.readAll());
    } else {
        return Response(request, "File not found!");
    }
}

void RequestHandler::run(){
    handle();
    condition->wakeOne();
}

QString RequestHandler::id(){
    return name;
}

RequestHandler::~RequestHandler(){
    delete mutex;
    delete condition;
}

int RequestHandler::counter = 0;
QString RequestHandler::baseName = "handler";
