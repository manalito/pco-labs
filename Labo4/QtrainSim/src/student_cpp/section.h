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
    QMutex* mutex;
    QSemaphore* inSection;
    QSemaphore* contact1;
    QSemaphore* contact2;
    bool libre;

    bool derivation;
    int numTrain1, numTrain2;

public:
    bool loco1_inside_section;
    bool loco2_inside_section;
    QPair<int, int> CriticSwitch1;
    QPair<int, int> CriticSwitch2;
    Section(QPair<int, int> CriticSwitch1, QPair<int, int> CriticSwitch2,
            int initSemInSection, int numTrain1, int numTrain2){
        libre = true;
        derivation = false;
        mutex = new QMutex();
        this->CriticSwitch1 = CriticSwitch1;
        this->CriticSwitch2 = CriticSwitch2;
        this->numTrain1 = numTrain1;
        this->numTrain2 = numTrain2;
        this->loco1_inside_section = false;
        this->loco2_inside_section = false;
        this->inSection = new QSemaphore(initSemInSection);
        this->contact1 = new QSemaphore(1);
        this->contact2 = new QSemaphore(1);

    }

    bool peutEntrer(int numLocomotive){

        afficher_message(qPrintable(QString("Loco %1 entering the critical Area!").arg(numLocomotive)));

        mutex->lock();

        bool resultat = false;
        if(libre){
            bloquer();
            libre = false;
            resultat = true;
        }else if (numLocomotive == numTrain1){
            derivation = true;
        }

        mutex->unlock();
        return resultat;
    }

    void sortir(int numLocomotive){
        mutex->lock();
        if(numLocomotive == numTrain1){
            /*diriger_aiguillage(CriticSwitch1.first, DEVIE, 0);
            diriger_aiguillage(CriticSwitch1.second, DEVIE, 0);
            diriger_aiguillage(CriticSwitch2.first, TOUT_DROIT, 0);
            diriger_aiguillage(CriticSwitch2.second, TOUT_DROIT, 0);*/

            //if(!derivation){
                libre = true;
                liberer();
            //}
            derivation = false;
        }
        if(numLocomotive == numTrain2){
            /*diriger_aiguillage(CriticSwitch1.first, TOUT_DROIT, 0);
            diriger_aiguillage(CriticSwitch2.first, DEVIE, 0);*/

            libre = true;
            liberer();

        }
        mutex->unlock();
        afficher_message(qPrintable(QString("Loco %1 exiting the critical Area!").arg(numLocomotive)));
    }

    void setLibre(bool val){
        libre = val;
    }

    void bloquer(){
        inSection->acquire();
    }

    void liberer(){
        inSection->release();
    }

    void locoInside(int numero){
        if(numero == numTrain1){
            loco1_inside_section = true;
        } else if(numero == numTrain2){
            loco2_inside_section = true;
        }
    }

    void locoOutside(int numero){
        if(numero == numTrain1){
            loco1_inside_section = false;
        } else if(numero == numTrain2){
            loco2_inside_section = false;
        }
    }

    void changeSwitch1(int numeroTrain){
        contact1->acquire();

        if(numeroTrain == numTrain2){
            diriger_aiguillage(CriticSwitch1.first, DEVIE, 0);
            diriger_aiguillage(CriticSwitch2.first, TOUT_DROIT, 0);
        }else if (derivation){
            diriger_aiguillage(CriticSwitch1.first, TOUT_DROIT, 0);
            diriger_aiguillage(CriticSwitch2.first, DEVIE, 0);
        } else{
            diriger_aiguillage(CriticSwitch1.first, DEVIE, 0);
            diriger_aiguillage(CriticSwitch2.first, DEVIE, 0);
        }
        contact1->release();
    }
    void changeSwitch2(int numeroTrain){
        contact2->acquire();

        if(numeroTrain == numTrain2){
            diriger_aiguillage(CriticSwitch1.second, DEVIE, 0);
            diriger_aiguillage(CriticSwitch2.second, TOUT_DROIT, 0);
        }else if (derivation){
            diriger_aiguillage(CriticSwitch1.second, TOUT_DROIT, 0);
            diriger_aiguillage(CriticSwitch2.second, DEVIE, 0);
        } else{
            diriger_aiguillage(CriticSwitch1.second, DEVIE, 0);
            diriger_aiguillage(CriticSwitch2.second, DEVIE, 0);
        }
        contact2->release();

    }
};

#endif // SECTION_H
