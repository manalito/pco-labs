/*
 *
 * @author Gallay Romain, Siu Aurélien
 *
 */

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
                  double scaleFactor, double centerX, double centerY, uint* colormap,
                  int ColormapSize, QImage* image, bool* restart, bool* abort, int threadId, const int NbThreads);
private:
    int halfWidth;
    int halfHeight;

    // these two variables helpus to scan the Qimage object
    int threadId;
    int NbThreads; // number of logical cores available on the system.
    int MaxIterations;

    double scaleFactor;
    double centerX;
    double centerY;

    uint* colormap;
    int ColormapSize;

    // These three variables are require to have pointer on, in order to work properly.
    QImage* image;
    bool* restart;
    bool* abort;

};
#endif // COMPUTETHREAD_H
