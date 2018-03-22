
#include "computethread.h"
#include <QWidget>

ComputeThread::ComputeThread(){
}

ComputeThread::ComputeThread(int halfHeight, int halfWidth,
                 double scaleFactor, const bool &restart, const bool &abort, QImage* image,
                 const int MaxIterations, double centerX,
                             double centerY, uint colormap[], int ColormapSize)
    : halfWidth(halfWidth), halfHeight(halfHeight), MaxIterations(MaxIterations),
      scaleFactor(scaleFactor), centerX(centerX), centerY(centerY), colormap(colormap),
      ColormapSize(ColormapSize), image(image), restart(&restart), abort(&abort){

}
void ComputeThread::compute(){
    const int Limit = 4;
    for (int y = -halfHeight; y < +halfHeight; ++y) {
        if (restart)
            break;
        if (abort)
            return;

        QRgb *scanLine =
                reinterpret_cast<QRgb *>(image->scanLine(y + halfHeight));
        double ay = centerY + (y * scaleFactor);

        for (int x = -halfWidth; x < +halfWidth; ++x) {
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

void ComputeThread::setArgs(int halfHeight, int halfWidth,
                            double scaleFactor, bool &restart, bool &abort, QImage* image,
                            const int Limit, const int MaxIterations,  double centerX, double centerY, uint* colormap, int colormapsize){
    this->halfHeight = halfHeight;
    this->halfWidth  = halfWidth;
    this->scaleFactor = scaleFactor;
    this->restart = &restart;
    this->abort = &abort;
    this->image = image;
    this->Limit = Limit;
    this->MaxIterations = (int)MaxIterations;
    this->centerX = centerX;
    this->centerY = centerY;
    this->colormap = colormap;
    this->ColormapSize = colormapsize;

}
