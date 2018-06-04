#ifndef RUNNABLE_H
#define RUNNABLE_H
#include <QString>
#include <QObject>

class Runnable : public QObject {
public:
virtual ~Runnable() {}
virtual void run() = 0;
virtual QString id() = 0;
};

#endif // RUNNABLE_H
