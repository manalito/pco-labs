#ifndef WORKER_H
#define WORKER_H

#include "locomotive.h"
#include "ctrain_handler.h"
#include "section.h"

#include <QObject>

/*
 * Worker class
 *
 */


/* Classe gérant les thread des locomotives. Ces thread ont une référence sur
 * la même instance de la zone critique et font appel à ses méthodes.
 *
 * Les méthodes départ() et arreter() sont utiles car le fait d'arrêter ou non
 * un thread n'a pas de conséquence "physique" sur les locomotives.
 *
 *
 * */
class Worker : public QThread{
private:
    Locomotive* loco;
    QList<int> course;
    QPair<int, int> criticalContact;
    Section* section;
    bool sens; //true = forward
    int turnNumber;
    int numNoPriorityLoco;
    QList<int> courseDev;
public:

    //Initialisation de la locomotive
    Worker(int id, int vitesse, QPair<int, int> startingPoint,
           bool phare, QList<int> course,
           QPair<int, int> criticalContact,
           Section* section, int numNoPriorityLoco){

        loco = new Locomotive();
        loco->fixerNumero(id);
        loco->fixerVitesse(vitesse);
        loco->fixerPosition(startingPoint.first, startingPoint.second);
        if(phare) loco->allumerPhares();
        this->course = course;
        this->criticalContact = criticalContact;
        this->section = section;
        this->numNoPriorityLoco = numNoPriorityLoco;
        sens = true;
        turnNumber = 0;
        loco->afficherMessage("Ready!");
    }

    void depart(){
        loco->demarrer();
        loco->afficherMessage(qPrintable(QString("The engine is starting!")));
    }
    void arreter(){
        loco->arreter();
        loco->afficherMessage(qPrintable(QString("The engine is stopping!")));
    }


    void run() Q_DECL_OVERRIDE{
        depart();

        /* Attente du passage sur les contacts. La zone critique n'a pas de début
         * ou de fin, mais des bornes (utile pour gérer les deux sens).
         *
         */
        int pos = 0;
        while(true){
            waitContact(course.at(pos));
            // Action à l'entrée de la zone critique
            if(course.at(pos) == criticalContact.first ||
                    course.at(pos) == criticalContact.second){
                section->locoInside(loco->numero());

                // Action spéciale pour la loco 2 si la zone est occupée:
                // elle s'arrête et attend que l'autre sorte de la zone
                //bool locoPeutEntrer = !section->peutEntrer(loco->numero());
                if(!section->peutEntrer(loco->numero()) &&
                        loco->numero() == numNoPriorityLoco){
                    arreter();
                    section->bloquer();
                    // Avant de repartir, la loco 2 indique qu'elle entre dans la zone critique
                    section->setLibre(false);
                    depart();
                }

                if(sens){
                    section->changeSwitch1(loco->numero());
                }else{
                    section->changeSwitch2(loco->numero());
                }
                pos = nextContact(pos);

                if(loco->numero() == numNoPriorityLoco){
                    waitContact(course.at(pos));
                    if(section->loco1_inside_section){
                        arreter();
                        section->bloquer();
                        section->setLibre(false);
                        depart();
                    }
                }
                pos = nextContact(pos);

                if(sens){
                    section->changeSwitch2(loco->numero());
                }else{
                    section->changeSwitch1(loco->numero());
                }

                if(loco->numero() == numNoPriorityLoco){
                    waitContact(course.at(pos));
                    if(section->loco1_inside_section){
                        arreter();
                        section->bloquer();
                        section->setLibre(false);
                        depart();
                    }
                    pos = nextContact(pos);
                } else {
                    pos = nextContact(pos);
                }

                waitContact(course.at(pos));
                section->sortir(loco->numero());
                section->locoOutside(loco->numero());
            }
            pos = nextContact(pos);
        }
        loco->afficherMessage(QString("Yeah, piece of cake for locomotive %1 !")
                              .arg(loco->numero()));
        arreter();
    }

    int nextContact(int pos){
        if(sens){
            pos++;
        }else{
            pos--;
        }
        if(pos >= course.size()){
            pos = 0;
            turnNumber++;
        }
        if(pos < 0){
            pos = course.size() - 1;
            turnNumber++;
        }
        if(turnNumber == 2){
            sens = !sens;
            turnNumber = 0;
            // On saute un capteur lors du changement de sens, pour ne pas rester bloqué
            pos = (sens ? 1 : course.size() - 2);
            loco->inverserSens();
        }
        return pos;
    }

    bool getSens(){
        return sens;
    }

    void waitContact(int contact){
        attendre_contact(contact);
        afficher_message(qPrintable(QString("The engine no. %1 has reached contact no. %2.")
                                    .arg(loco->numero()).arg(contact)));
        loco->afficherMessage(QString("I've reached contact no. %1.").arg(contact));
    }
};

#endif // WORKER_H
