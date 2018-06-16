#include "readerwritercache.h"
#include "option.h"
#include <QDateTime>
#include <QList>
#include <QDebug>

ReaderWriterCache::ReaderWriterCache(int invalidationDelaySec, int staleDelaySec):
    invalidationDelaySec(invalidationDelaySec), staleDelaySec(staleDelaySec)
{
    timer = new InvalidationTimer(this);
}

ReaderWriterCache::~ReaderWriterCache(){
    delete timer;
}

void ReaderWriterCache::putResponse(Response &response) {
    lock.lockWriting();

    TimestampedResponse tr;
    tr.response = response;
    tr.timestamp = QDateTime::currentSecsSinceEpoch();
    map.insert(response.getRequest().getFilePath(), tr);

    QTextStream(stdout) << "entry put in cache" << endl;

    lock.unlockWriting();
}

Option<Response> ReaderWriterCache::tryGetCachedResponse(Request &request) {
    lock.lockReading();
    if(map.contains(request.getFilePath())){
        TimestampedResponse tr = map.value(request.getFilePath());
        lock.unlockReading();

        QTextStream(stdout) << "entry found in cache" << endl;

        return Option<Response>::some(tr.response);
    }
    else{
        lock.unlockReading();
        return Option<Response>::none();
}
}
