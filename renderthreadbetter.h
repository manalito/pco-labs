#ifndef RENDERTHREADBETTER_H
#define RENDERTHREADBETTER_H

#include <QMutex>
#include <QSize>
#include <QThread>
#include <QWaitCondition>

QT_BEGIN_NAMESPACE
class QImage;
QT_END_NAMESPACE

//! [0]
class RenderThreadBetter : public RenderThread
{
    Q_OBJECT

public:
    RenderThreadBetter(QObject *parent = 0);
    ~RenderThreadBetter();

    void render(double centerX, double centerY, double scaleFactor, QSize resultSize);

signals:
    void renderedImage(const QImage &image, double scaleFactor);

protected:
    void run();

private:
    uint rgbFromWaveLength(double wave);

    QMutex mutex;
    QWaitCondition condition;
    double centerX;
    double centerY;
    double scaleFactor;
    QSize resultSize;
    bool restart;
    bool abort;

    enum { ColormapSize = 512 };
    uint colormap[ColormapSize];
};
//! [0]

#endif // RENDERTHREADBETTER_H
