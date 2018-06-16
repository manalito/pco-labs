#include <QDebug>
#include "requestprocessor.h"
#include "filereader.h"

RequestProcessor::RequestProcessor(Request request, AbstractBuffer<Response>* responses,
                                       bool hasDebugLog, ReaderWriterCache* cache):
                            request(request), responses(responses),
                            hasDebugLog(hasDebugLog), cache(cache) {}

void RequestProcessor::run()
{
    // retrieve the path from the reader
    FileReader reader(request.getFilePath(), hasDebugLog);

    // if the path corresponds to a file, read it and put the result in response
    if (reader.fileExists()) {
        Response response(request, reader.readAll());
        responses->put(response);
        cache->putResponse(response);
    } else {
        Response response(request, "File not found!");
        responses->put(response);
    }
}
