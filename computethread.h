#ifndef COMPUTETHREAD_H
#define COMPUTETHREAD_H

#include <QThread>

class ComputeThread : public QThread {
private:
    int minHeight;
    int maxHeight;
    int minWidth;
    int maxWidth;
    double scaleFactor;
    bool restart;
    bool abort;
    double centerX;
    double centerY;
    enum { ColormapSize = 512 };
    uint colormap[ColormapSize];
    QImage* image;
    virtual void run();

public:
    void compute();
    void setArgs(int minHeight, int maxHeight, int minWidth, int maxWidth,
                 double scaleFactor, bool restart, bool abort, QImage* image, const int Limit);
};

#endif // COMPUTETHREAD_H
