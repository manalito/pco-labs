#ifndef SECTION_H
#define SECTION_H

#include <QList>
#include <QString>
#include <QMutex>

#include "locomotive.h"
#include "ctrain_handler.h"

/*
* Classe gérant la zone critique de la maquette
 *
 * Les deux loco avertissent quand la zone est occupée, mais elles
 * ont un comportement différent dans celle-ci. Si la zone est libre
 * lorsque la loco 1 arrive, elle entre normalement et empêche la 2
 * d'y entrer. Dans l'autre situation, si la 2 entre dans la zone critique
 * avant la 1, cela ne bloque pas la 1, mais active la dérivation, donc la
 * mise en parallèle des deux loco.
 *
 * A la sortie de la zone critique, la loco 2 remet la zone comme étant
 * libre (dans tous les cas) et pour la loco 1, elle le fait uniquement
 * si elle n'a pas été déviée. Donc pas de problème de concurrence sur la
 * variable libre lorsque les deux loco sont en parallèle dans la zone critique
 *
 * Si la loco 2 a été arrêtée, elle met la zone comme non-libre seulement après
 * être repartie, donc après que la loco 1 ait libéré la zone. Il n'y a donc
 * ici non plus de problème de concurrence sur la variable libre.
 *
 *
 */
class Section{
private:
    QMutex* mutex;
    QSemaphore* sem;
    bool libre;
    QPair<int, int> aiguillageCrit1;
    QPair<int, int> aiguillageCrit2;
    bool derivation;
    int numTrain1;
    int numTrain2;

public:
    Section(QPair<int, int> aiguillageCrit1,
                 QPair<int, int> aiguillageCrit2, QSemaphore* sem, int numTrain1, int numTrain2){
        libre = true;
        derivation = false;
        mutex = new QMutex();
        this->sem = sem;
        this->aiguillageCrit1 = aiguillageCrit1;
        this->aiguillageCrit2 = aiguillageCrit2;
        this->numTrain1 = numTrain1;
        this->numTrain2 = numTrain2;
    }

    bool peutEntrer(int numLocomotive){

        afficher_message(qPrintable(QString("Entering the critical Area!")));
        mutex->lock();
        bool resultat = false;
        if(libre){
            if(numLocomotive == numTrain1){
                diriger_aiguillage(aiguillageCrit1.first, TOUT_DROIT, 0);
                diriger_aiguillage(aiguillageCrit1.second, TOUT_DROIT, 0);
            }
            bloquer();
            libre = false;
            resultat = true;
        }
        else{
            if(numLocomotive == numTrain1){
                diriger_aiguillage(aiguillageCrit2.first, TOUT_DROIT, 0);
                diriger_aiguillage(aiguillageCrit2.second, TOUT_DROIT, 0);
                derivation = true;
            }
        }
        mutex->unlock();
        return resultat;
    }

    void sortir(int numLocomotive){
        if(numLocomotive == numTrain1){
            diriger_aiguillage(aiguillageCrit1.first, DEVIE, 0);
            diriger_aiguillage(aiguillageCrit1.second, DEVIE, 0);
            diriger_aiguillage(aiguillageCrit2.first, DEVIE, 0);
            diriger_aiguillage(aiguillageCrit2.second, DEVIE, 0);

            if(!derivation){
                libre = true;
                liberer();
            }
            derivation = false;
        }
        if(numLocomotive == numTrain2){
            libre = true;
            liberer();

        }
        afficher_message(qPrintable(QString("Exiting the critical Area!")));
    }

    void setLibre(bool val){
        libre = val;
    }

    void bloquer(){
        sem->acquire();
    }

    void liberer(){
        sem->release();
    }
};

#endif // SECTION_H
