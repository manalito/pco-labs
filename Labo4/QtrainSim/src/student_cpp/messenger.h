#ifndef MESSENGER_H
#define MESSENGER_H

#include <QMutex>
#include "worker.h"

class messenger : public QThread {

private:

    QString command;
    Worker* w1;
    Worker* w2;

public:

    messenger(Worker* w1, Worker* w2) {
        this->w1 = w1;
        this->w2 = w2;
    }

    void run() Q_DECL_OVERRIDE{
        while(true){
            command = getCommand();

            if(command.compare("loco1") == 0){
                w2->setOtherLocoPriority(true);
                afficher_message(qPrintable(QString("Loco 1 has the priority")));
            } else if(command.compare("loco2") == 0){
                w1->setOtherLocoPriority(true);
                afficher_message(qPrintable(QString("Loco 2 has the priority")));
            } else if(command.compare("reset") == 0){
                w1->setOtherLocoPriority(false);
                w2->setOtherLocoPriority(false);
                afficher_message(qPrintable(QString("Both priorities have been reset")));
            } else {
                afficher_message(qPrintable(QString("Syntaxe : loco1, loco2, reset")));
            }
        }
    }








};

#endif // MESSENGER_H
