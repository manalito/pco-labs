#include <QDebug>
#include "requesthandler.h"
#include "filereader.h"

RequestHandler::RequestHandler(Request request, AbstractBuffer<Response>* responses,
                               bool hasDebugLog): request(request),
                               responses(responses), hasDebugLog(hasDebugLog){
    ++counter;
    name = baseName + counter;
}

RequestHandler::RequestHandler(const RequestHandler &handler){
    request = handler.request;
    hasDebugLog = handler.hasDebugLog;
}

void RequestHandler::handle()
{
    qInfo() << "starting handling" << endl;
    FileReader reader(request.getFilePath(), hasDebugLog);
    // if the path corresponds to a file, read it and put the result in response
    if (reader.fileExists()) {
        Response response(request, reader.readAll());
        responses->put(response);
    } else {
        Response response(request, "File not found!");
        responses->put(response);
    }
    qInfo() << "handled" << endl;
}

void RequestHandler::run(){
    qInfo() << "processing..." << endl;
    handle();
}

QString RequestHandler::id(){
    return name;
}

RequestHandler::~RequestHandler(){
}

int RequestHandler::counter = 0;
QString RequestHandler::baseName = "handler";
