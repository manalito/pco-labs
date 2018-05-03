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
    QList<int> courseDerivation;
    QList<int> criticalContact;
    QList<int> criticalDerivation;
    Section* section;
    bool sens; //true = forward
    int turnNumber;
    int loco1Number;
    int loco2Number;
    bool derivation = false;
public:

    //Initialisation de la locomotive
    //Id 1 corresponds to the loco that takes the Derivation (small route)
    //Id 2 corresponds to the loco that stops (big route)
    Worker(int id, int vitesse, QPair<int, int> startingPoint,
                 bool phare, QList<int> course,
                 QList<int> criticalContact,
                 Section* section, int loco1Number, int loco2Number,
                 QList<int> courseDerivation, QList<int> criticalDerivation){
        loco = new Locomotive();
        loco->fixerNumero(id);
        loco->fixerVitesse(vitesse);
        loco->fixerPosition(startingPoint.first, startingPoint.second);
        if(phare) loco->allumerPhares();
        this->course = course;
        this->criticalContact = criticalContact;
        this->section = section;
        this->loco1Number = loco1Number;
        this->loco2Number = loco2Number;
        this->courseDerivation = courseDerivation;
        this->criticalDerivation = criticalDerivation;
        sens = true;
        turnNumber = 0;
        loco->afficherMessage("Ready!");
    }

    // apply the proper behavior (stop or deviate) if both loco are in the section
    void conflictManagment(bool earlyCheck, int switchNumber){

        if(!earlyCheck){

            afficher_message(qPrintable(QString("no earyl check")));
            if(section->getloco1Inside()){
                afficher_message(qPrintable(QString("should stop")));
                arreter();
                section->bloquer();
                depart();
                section->liberer();
            }
        } else{

            afficher_message(qPrintable(QString("earyl check")));
            if(switchNumber == 1){
                if(section->getloco1Inside() || section->getaboutToCrossSwitch1()){
                    afficher_message(qPrintable(QString("should stop")));
                    arreter();
                    section->bloquer();
                    depart();
                    section->liberer();
                }
            } else {
                if(section->getloco1Inside() || section->getaboutToCrossSwitch2()){
                    afficher_message(qPrintable(QString("should stop")));
                    arreter();
                    section->bloquer();
                    depart();
                    section->liberer();
                }

            }

        }

    }

    void run() Q_DECL_OVERRIDE{
        depart();

        int pos = 0;
        while(true){

            if(loco->numero() == loco2Number){
                waitContact(course.at(pos));

                if(course.at(pos) == criticalContact.at(0)){
                    // about to enter the critical zone
                    conflictManagment(false, 1);

                } else if (course.at(pos) == criticalContact.at(1)){
                    // entering the critical zone

                    section->setloco2Inside(true);
                    conflictManagment(true, 1);

                    // change switches
                    section->changeSwitch(sens, loco->numero(), false);

                } else if (course.at(pos) == criticalContact.at(2)){
                    // 1st contact inside critical zone
                    conflictManagment(true, 2);

                } else if (course.at(pos) == criticalContact.at(3)){
                    // 2nd contact inside critical zone
                    conflictManagment(true, 2);
                    // change switches
                    section->changeSwitch(!sens, loco->numero(), false);

                } else if (course.at(pos) == criticalContact.at(4)){
                    // exiting critical zone
                    section->setloco2Inside(false);

                }

                pos = nextContact(pos);


            } else if(loco->numero() == loco1Number){

                if(derivation){
                    waitContact(courseDerivation.at(pos));
                } else {
                    waitContact(course.at(pos));
                }

                if(course.at(pos) == criticalContact.at(0)){
                    // about to enter the critical zone
                    section->setaboutToCrossSwitch1(true);
                    section->bloquer();

                } else if (course.at(pos) == criticalContact.at(1)){
                    // entering the critical zone
                    section->setloco1Inside(true);
                    section->setaboutToCrossSwitch1(false);

                    if(section->getloco2Inside()){
                        derivation = true;
                    }
                    section->changeSwitch(sens, loco->numero(), derivation);

                } else if (course.at(pos) == criticalContact.at(2)){
                    // 1st contact inside critical zone
                    section->setaboutToCrossSwitch2(true);

                } else if (course.at(pos) == criticalContact.at(3)){
                    // 2nd contact inside critical zone
                    section->changeSwitch(!sens, loco->numero(), derivation);

                } else if (course.at(pos) == criticalContact.at(4)){
                    // exiting critical zone
                    section->setaboutToCrossSwitch2(false);
                    section->setloco1Inside(false);
                    section->liberer();
                    derivation = false;

                }
                pos = nextContact(pos);

            } else {
                afficher_message(qPrintable(QString("Error setting loco numbers")));
            }
        }
        loco->afficherMessage(QString("Yeah, piece of cake for locomotive %1 !")
                                    .arg(loco->numero()));
        arreter();
    }

    /*
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
    }*/

    int nextContact(int pos){
        pos++;
        if(pos >= course.size()){
           pos = 0;
           turnNumber++;
        }

        if(turnNumber == 2){
            turnNumber = 0;
            afficher_message(qPrintable(QString("1st contact before = %1").arg(course.at(1))));
            reverseQLists();
            afficher_message(qPrintable(QString("1st contact after = %1").arg(course.at(1))));
            sens = !sens;
            pos = 1;
            loco->inverserSens();

            if(loco->numero() == loco1Number && !sens){
                // because we cannot re enter correctly after switch direction
                afficher_message(qPrintable(QString("tryna block ?")));
                section->bloquer();
                afficher_message(qPrintable(QString("did block !")));
            }
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

    void depart(){
        loco->demarrer();
        loco->afficherMessage(qPrintable(QString("The engine is starting!")));
    }
    void arreter(){
        loco->arreter();
        loco->afficherMessage(qPrintable(QString("The engine is stopping!")));
    }

    void reverseQLists(){

        for(int k = 0; k < (course.size()/2); k++) {
            course.swap(k,course.size()-(1+k));
        }

        for(int k = 0; k < (courseDerivation.size()/2); k++) {
            courseDerivation.swap(k,courseDerivation.size()-(1+k));
        }

        for(int k = 0; k < (criticalContact.size()/2); k++) {
            criticalContact.swap(k,criticalContact.size()-(1+k));
        }
    }
};

#endif // WORKER_H
