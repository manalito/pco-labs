
#include "computethread.h"
#include <QWidget>


void ComputeThread::compute(){
    for (int y = minHeight; y < maxHeight; ++y) {
        if (restart)
            break;
        if (abort)
            return;

        QRgb *scanLine =
                reinterpret_cast<QRgb *>(image->scanLine(y + maxHeight));
        double ay = centerY + (y * scaleFactor);

        for (int x = minWidth; x < maxHeight; ++x) {
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
void ComputeThread::run() {
    this->compute();
}

void ComputeThread::setArgs(int minHeight, int maxHeight, int minWidth, int maxWidth,
                            double scaleFactor, bool restart, bool abort, QImage* image,
                            const int Limit, const int MaxIterations,  double centerX, double centerY){
    this->minHeight = minHeight;
    this->maxHeight = maxHeight;
    this->minWidth  = minWidth;
    this->maxWidth  = maxWidth;
    this->scaleFactor = scaleFactor;
    this->restart = restart;
    this->abort = abort;
    this->image = image;
    this->Limit = Limit;
    this->MaxIterations = MaxIterations;
    this->centerX = centerX;
    this->centerY = centerY;

}
