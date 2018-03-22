#ifndef CALCULTHREAD_H
#define CALCULTHREAD_H

#include <QThread>
#include <QImage>

class ComputeThread : public QThread
{

public:
    ComputeThread();
    void run();
    ComputeThread(int halfWidth, int halfHeight, int MaxIterations,
                  double scaleFactor, double centerX, double centerY, uint colormap[],
                  int ColormapSize, QImage* image, bool* restart, bool* abort, int threadId, const int NbThreads);

private:
    int halfWidth;
    int halfHeight;

    int threadId;
    int NbThreads;
    int MaxIterations;

    double scaleFactor;
    double centerX;
    double centerY;

    uint* colormap;
    int ColormapSize;

    QImage* image;
    bool* restart;
    bool* abort;

};
#endif // COMPUTETHREAD_H
