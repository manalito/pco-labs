#ifndef COMPUTETHREAD_H
#define COMPUTETHREAD_H

#include <QThread>

class ComputeThread : public QThread {
private:
    int halfHeight;
    int halfWidth;
    double scaleFactor;
    const bool* restart;
    const bool* abort;
    double centerX;
    double centerY;

    int ColormapSize;
    uint* colormap;
    QImage* image;
    int Limit;
    int MaxIterations;
    virtual void run();

public:
    ComputeThread();
    ComputeThread(int halfHeight, int halfWidth, double scaleFactor,const bool &restart,
                  const bool &abort, QImage* image, int MaxIterations, double centerX,
                  double centerY, uint colormap[], int colormapsize);
    void compute();
    void setArgs(int halfHeight, int halfWidth,
                 double scaleFactor, bool &restart, bool &abort, QImage* image,
                 const int Limit, const int MaxIterations, double centerX,
                 double centerY, uint* colormap, int colormapsize);
};

#endif // COMPUTETHREAD_H
