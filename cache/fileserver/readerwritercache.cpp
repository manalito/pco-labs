#include "readerwritercache.h"
#include "option.h"
#include <QDateTime>
#include <QList>
#include <QDebug>

ReaderWriterCache::ReaderWriterCache(int invalidationDelaySec, int staleDelaySec):
    invalidationDelaySec(invalidationDelaySec), staleDelaySec(staleDelaySec)
{
    timer = new InvalidationTimer(this);
    timer->start();
}

ReaderWriterCache::~ReaderWriterCache(){
    timer->requestInterruption();
    timer->wait();
    delete timer;
}

void ReaderWriterCache::putResponse(Response &response) {
    lock.lockWriting();
    TimestampedResponse timestampedResponse;
    timestampedResponse.response = response;
    timestampedResponse.timestamp = QDateTime::currentSecsSinceEpoch();
    map.insert(response.getRequest().getFilePath(), timestampedResponse);
    lock.unlockWriting();
}

Option<Response> ReaderWriterCache::tryGetCachedResponse(Request &request) {
    lock.lockReading();

    // if the map contains the response, return an option with this response
    if(map.contains(request.getFilePath())){
        lock.unlockReading();
        return Option<Response>::some(map.value(request.getFilePath()).response);
    }
    // otherwise, return an empty option
    else{
        lock.unlockReading();
        return Option<Response>::none();
    }
}
