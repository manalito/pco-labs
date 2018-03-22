#include "computeThread.h"

ComputeThread::ComputeThread(){}


ComputeThread::ComputeThread(int halfWidth, int halfHeight, int MaxIterations,
                             double scaleFactor, double centerX, double centerY, uint colormap[],
                             int ColormapSize, QImage* image, bool* restart, bool* abort,
                             int threadId, const int NbThreads):
    halfWidth(halfWidth), halfHeight(halfHeight), MaxIterations(MaxIterations),
    scaleFactor(scaleFactor), centerX(centerX), centerY(centerY), colormap(colormap),
    ColormapSize(ColormapSize), image(image), restart(restart), abort(abort), threadId(threadId), NbThreads(NbThreads)
{}

void ComputeThread::run(){

    const int Limit = 4;
    for (int y = -halfHeight + threadId; y < halfHeight; y+=NbThreads) {
        if (*restart) {
            break;
        }
        if (*abort) {
            return;
        }
        QRgb *scanLine = reinterpret_cast<QRgb *>(image->scanLine(y + halfHeight));
        double ay = centerY + (y * scaleFactor);

        for (int x = -halfWidth; x < halfWidth; ++x) {

            double ax = centerX + (x * scaleFactor);
            double a1 = ax;
            double b1 = ay;
            int numIterations = 0;

            do {
                ++numIterations;
                double a2 = (a1 * a1) - (b1 * b1) + ax;
                double b2 = (2 * a1 * b1) + ay;
                if ((a2 * a2) + (b2 * b2) > Limit)
                    break;

                ++numIterations;
                a1 = (a2 * a2) - (b2 * b2) + ax;
                b1 = (2 * a2 * b2) + ay;
                if ((a1 * a1) + (b1 * b1) > Limit)
                    break;
            } while (numIterations < MaxIterations);

            if (numIterations < MaxIterations) {
                *scanLine++ = colormap[numIterations % ColormapSize];
            } else {
                *scanLine++ = qRgb(0, 0, 0);
            }
        }
    }
}
