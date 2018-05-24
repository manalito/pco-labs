#include "requestdispatcherthread.h"

void RequestDispatcherThread::run()
{
    while(true) {
        if (hasDebugLog)
            qDebug() << "Waiting for requests...";
        Request req = requests->get();   // block until a response is available
        // TODO send requests
    }
}
