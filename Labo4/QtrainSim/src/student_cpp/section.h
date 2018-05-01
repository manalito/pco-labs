#ifndef SECTION_H
#define SECTION_H

#include <QList>
#include <QString>
#include <QMutex>

#include "locomotive.h"
#include "ctrain_handler.h"

class Section{
private:
    QMutex* mutex;
    QSemaphore* semaphore;
    bool libre;

    bool derivation;
    int numTrain1, numTrain2;

public:
    QPair<int, int> CriticSwitch1;
    QPair<int, int> CriticSwitch2;
    Section(QPair<int, int> CriticSwitch1, QPair<int, int> CriticSwitch2,
            QSemaphore* sem, int numTrain1, int numTrain2){
        libre = true;
        derivation = false;
        mutex = new QMutex();
        this->semaphore = sem;
        this->CriticSwitch1 = CriticSwitch1;
        this->CriticSwitch2 = CriticSwitch2;
        this->numTrain1 = numTrain1;
        this->numTrain2 = numTrain2;
    }

    bool peutEntrer(int numLocomotive){

        afficher_message(qPrintable(QString("Entering the critical Area!")));

        mutex->lock();

        bool resultat = false;
        if(libre){
            if(numLocomotive == numTrain1){
                diriger_aiguillage(CriticSwitch1.first, DEVIE, 0);
                diriger_aiguillage(CriticSwitch1.second, DEVIE, 0);
                diriger_aiguillage(CriticSwitch2.first, DEVIE, 0);
                diriger_aiguillage(CriticSwitch2.first, DEVIE, 0);
            }
            bloquer();
            libre = false;
            resultat = true;
        }
        else{
            if(numLocomotive == numTrain1){

                diriger_aiguillage(CriticSwitch1.second, TOUT_DROIT, 0);
                diriger_aiguillage(CriticSwitch2.second, DEVIE, 0);

                derivation = true;
            }
            if(numLocomotive == numTrain2){

                diriger_aiguillage(CriticSwitch1.second, TOUT_DROIT, 0);
                diriger_aiguillage(CriticSwitch2.second, DEVIE, 0);


            }
        }
        mutex->unlock();
        return resultat;
    }

    void sortir(int numLocomotive){
        mutex->lock();
        if(numLocomotive == numTrain1){
            diriger_aiguillage(CriticSwitch1.first, DEVIE, 0);
            diriger_aiguillage(CriticSwitch1.second, DEVIE, 0);
            diriger_aiguillage(CriticSwitch2.first, TOUT_DROIT, 0);
            diriger_aiguillage(CriticSwitch2.second, TOUT_DROIT, 0);

            if(!derivation){
                libre = true;
                liberer();
            }
            derivation = false;
        }
        if(numLocomotive == numTrain2){
            diriger_aiguillage(CriticSwitch1.first, TOUT_DROIT, 0);
            diriger_aiguillage(CriticSwitch2.first, DEVIE, 0);

            libre = true;
            liberer();

        }
        mutex->unlock();
        afficher_message(qPrintable(QString("Exiting the critical Area!")));
    }

    void setLibre(bool val){
        libre = val;
    }

    void bloquer(){
        semaphore->acquire();
    }

    void liberer(){
        semaphore->release();
    }
};

#endif // SECTION_H
