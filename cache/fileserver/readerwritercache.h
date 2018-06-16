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
#include <QDebug>

// This class implements a cache mechanism based on ReaderWriterLock and QHash
class ReaderWriterCache
{
private:
    // struct containing a response and a timestamp to determine its validity
	struct TimestampedResponse {
		Response response;
		long timestamp;
	};

    // a class implementing a timer to periodically delete map entries when
    // their validity time is expired
	class InvalidationTimer: public QThread {
		friend ReaderWriterCache;

	private:
        // cache to be checked
		ReaderWriterCache* cache;

	public:
        // constructor
		InvalidationTimer(ReaderWriterCache* cache): cache(cache) {}

	protected:
		void run() {
            forever{
                /* we only check the map every "invalidationDelaySec" to avoid
                 * overloading the CPU
                 */
                sleep(cache->invalidationDelaySec);

                long currentTime = QDateTime::currentSecsSinceEpoch();
                cache->lock.lockWriting();
                // iterate on the map
                for(QHash<QString, TimestampedResponse>::iterator i=cache->map.begin();
                    i!=cache->map.end(); ++i){
                    // if an entry is expired, delete it from the map
                    if(cache->staleDelaySec < currentTime - i.value().timestamp){
                        cache->map.remove(i.key());
                    }
                }
                cache->lock.unlockWriting();

                /* check if an interruption is requested to terminate the thread
                 * properly
                 */
                if(QThread::currentThread()->isInterruptionRequested())
                    return;
            }
		}
	};
    /* hash-table containing a request's file path and the corresponding
     * TimestampedResponse
     */
	QHash<QString, TimestampedResponse> map;
    // verification interval to check the map for stale entries
	int invalidationDelaySec;
    // maximum period of validity of entries
	int staleDelaySec;
    // timer to check the entries validity
	InvalidationTimer* timer;
    // lock to manage concurrent accesses in the cache
	ReaderWriterLock lock;

public:
    // constructor
	ReaderWriterCache(int invalidationDelaySec, int staleDelaySec);
    /* returns an option containing the response to the given requests if its
     * in the cache, returns an empty option otherwise
     */
	Option<Response> tryGetCachedResponse(Request& request);
    // write a response in the cache
	void putResponse(Response& response);
    // destructor
    ~ReaderWriterCache();
};

#endif // READERWRITERCACHE_H
