#ifndef SECTION_H
#define SECTION_H

#include <QList>
#include <QString>
#include <QMutex>
#include <QSemaphore>

#include "locomotive.h"
#include "ctrain_handler.h"

class Section{
private:
    int loco1Number;
    int loco2Number;
    QMutex* m1 = new QMutex();
    QMutex* m2 = new QMutex();
    QSemaphore* busy = new QSemaphore(1);
    QSemaphore* switch1 = new QSemaphore(1);
    QSemaphore* switch2 = new QSemaphore(1);
    bool aboutToCrossSwitch1 = false;
    bool loco1Inside = false;
    bool aboutToCrossSwitch2 = false;
    bool loco2Inside = false;

public:

    Section(int loco1Number, int loco2Number){
        this->loco1Number = loco1Number;
        this->loco2Number = loco2Number;
    }

    void changeSwitch(bool sens, int locoNumber, bool deviation){

        if(sens){ // bottom switch
            switch1->acquire();

            if(locoNumber == loco1Number){

                if(deviation){
                    afficher_message(qPrintable(QString("sw1, deriv")));
                    diriger_aiguillage(8, DEVIE, 0);
                    diriger_aiguillage(7, TOUT_DROIT,  0);
                } else {
                    afficher_message(qPrintable(QString("sw1, no deriv")));
                    diriger_aiguillage(8, DEVIE, 0);
                    diriger_aiguillage(7, DEVIE,  0);
                }
            } else if(locoNumber == loco2Number){
                afficher_message(qPrintable(QString("Im about to cross switch1")));
                diriger_aiguillage(8, TOUT_DROIT, 0);
                diriger_aiguillage(7, DEVIE,  0);
            }
            switch1->release();

        } else { // top switch
            switch2->acquire();

            if(locoNumber == loco1Number){

                if(deviation){
                    afficher_message(qPrintable(QString("sw2, deriv")));
                    diriger_aiguillage(4, TOUT_DROIT, 0);
                    diriger_aiguillage(3, DEVIE,  0);
                } else {
                    afficher_message(qPrintable(QString("sw2, no deriv")));
                    diriger_aiguillage(4, DEVIE, 0);
                    diriger_aiguillage(3, DEVIE,  0);
                }
            } else if(locoNumber == loco2Number){
                afficher_message(qPrintable(QString("Im about to cross switch2")));
                diriger_aiguillage(3, TOUT_DROIT, 0);
                diriger_aiguillage(4, DEVIE,  0);
            }
            switch2->release();

        }
    }

    void bloquer(){
        busy->acquire();
        afficher_message(qPrintable(QString("bloqued")));
    }

    void liberer(){
        busy->release();
        afficher_message(qPrintable(QString("released")));
    }

    void setaboutToCrossSwitch1(bool b){
        aboutToCrossSwitch1 = b;
    }

    void setaboutToCrossSwitch2(bool b){
        aboutToCrossSwitch2 = b;
    }

    void setloco1Inside(bool b){
        loco1Inside = b;
    }

    void setloco2Inside(bool b){
        loco2Inside = b;
    }

    bool getloco1Inside(){
        return loco1Inside;
    }

    bool getloco2Inside(){
        return loco2Inside;
    }

    bool getaboutToCrossSwitch1(){
        return aboutToCrossSwitch1;
    }

    bool getaboutToCrossSwitch2(){
        return aboutToCrossSwitch1;
    }

    void waitFreeSignal(){
       busy->acquire();
    }
};

#endif // SECTION_H
