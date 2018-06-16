#ifndef READERWRITERCACHE_H
#define READERWRITERCACHE_H
#include <QDateTime>
#include <QHash>
#include <QThread>
#include <QMutex>
#include "option.h"
#include "request.h"
#include "response.h"
#include "readerwriterlock.h"

class ReaderWriterCache
{
private:
	struct TimestampedResponse {
		Response response;
		long timestamp;
	};

	class InvalidationTimer: public QThread {
		friend ReaderWriterCache;

	private:
		ReaderWriterCache* cache;

	public:
		InvalidationTimer(ReaderWriterCache* cache): cache(cache) {}

	protected:
		void run() {
            forever{
                sleep(cache->invalidationDelaySec);

                long currentTime = QDateTime::currentSecsSinceEpoch();
                cache->lock.lockWriting();
                for(QHash<QString, TimestampedResponse>::iterator i=cache->map.begin(); i!=cache->map.end(); ++i){
                    if(cache->invalidationDelaySec < currentTime - i.value().timestamp){
                        cache->map.remove(i.key());
                    }
                }
                cache->lock.unlockWriting();
            }
		}
	};

	QHash<QString, TimestampedResponse> map;
	int invalidationDelaySec;
	int staleDelaySec;
	InvalidationTimer* timer;
	ReaderWriterLock lock;

public:
	ReaderWriterCache(int invalidationDelaySec, int staleDelaySec);
	Option<Response> tryGetCachedResponse(Request& request);
	void putResponse(Response& response);
    ~ReaderWriterCache();
};

#endif // READERWRITERCACHE_H
