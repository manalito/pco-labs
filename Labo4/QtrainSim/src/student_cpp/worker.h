#ifndef WORKER_H
#define WORKER_H

#include "locomotive.h"
#include "ctrain_handler.h"
#include "section.h"

#include <QObject>

class Worker : public QThread{
private:
    Locomotive* loco;
    QList<int> course;
    QList<int> courseDerivation;
    QList<int> criticalContact;
    QPair<int, int> startingPoint;
    Section* section;
    bool sens; //true = forward
    int turnNumber;
    int loco1Number;
    int loco2Number;
    bool derivation = false;
    bool otherLocoHasPriority;
public:

    //Initialisation de la locomotive
    //Id 1 corresponds to the loco that takes the Derivation (small route)
    //Id 2 corresponds to the loco that stops (big route)
    Worker(int id, int vitesse, QPair<int, int> startingPoint,
                 bool phare, QList<int> course,
                 QList<int> criticalContact,
                 Section* section, int loco1Number, int loco2Number,
                 QList<int> courseDerivation, bool otherLocoHasPriority){
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
        this->otherLocoHasPriority = otherLocoHasPriority;
        this->startingPoint = startingPoint;
        sens = true;
        turnNumber = 0;
        loco->afficherMessage("Ready!");
    }

    // apply the proper behavior (stop or deviate) if both loco are in the section
    void conflictManagment(bool earlyCheck, bool sens){

        if(!earlyCheck){

            if(section->getloco1Inside()){
                stop();
            }
        } else{

            if(sens){
                if(section->getloco1Inside() || section->getaboutToCrossSwitch1()){
                    stop();
                }
            } else {
                if(section->getloco1Inside() || section->getaboutToCrossSwitch2()){
                    stop();
                }
            }
        }
    }

    void run() Q_DECL_OVERRIDE{
        depart();
        int pos = course.indexOf(startingPoint.first);
        while(true){

            if(loco->numero() == loco2Number){
                waitContact(course.at(pos));

                if(course.at(pos) == criticalContact.at(0)){
                    // about to enter the critical zone
                    conflictManagment(false, sens);

                } else if (course.at(pos) == criticalContact.at(1)){
                    // entering the critical zone

                    if(otherLocoHasPriority){
                        arreter();
                        while(otherLocoHasPriority);
                        depart();
                    }
                    section->setloco2Inside(true);
                    conflictManagment(true, sens);
                    section->changeSwitch(sens, loco->numero(), false);

                } else if (course.at(pos) == criticalContact.at(2)){
                    // 1st contact inside critical zone
                    conflictManagment(true, !sens);

                } else if (course.at(pos) == criticalContact.at(3)){
                    // 2nd contact inside critical zone
                    conflictManagment(true, !sens);
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
                    section->acquire();

                } else if (course.at(pos) == criticalContact.at(1)){
                    // entering the critical zone
                    section->setloco1Inside(true);
                    section->setaboutToCrossSwitch1(false);
                    section->setaboutToCrossSwitch2(false);

                    if(section->getloco2Inside() || otherLocoHasPriority){
                        derivation = true;
                    }
                    section->changeSwitch(sens, loco->numero(), derivation);

                } else if (course.at(pos) == criticalContact.at(2)){
                    // 1st contact inside critical zone
                    if(sens){
                        section->setaboutToCrossSwitch2(true);
                    } else {
                        section->setaboutToCrossSwitch1(true);
                    }

                } else if (course.at(pos) == criticalContact.at(3)){
                    // 2nd contact inside critical zone
                    section->changeSwitch(!sens, loco->numero(), derivation);

                } else if (course.at(pos) == criticalContact.at(4)){
                    // exiting critical zone
                    section->setaboutToCrossSwitch1(false);
                    section->setaboutToCrossSwitch2(false);
                    section->setloco1Inside(false);
                    section->release();
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

    int nextContact(int pos){
        pos++;
        if(pos >= course.size()){
           pos = 0;
           turnNumber++;
        }

        if(turnNumber == 2){
            turnNumber = 0;
            reverseQLists();
            sens = !sens;
            pos = 1;
            loco->inverserSens();
            // because we cannot re enter after switching in backward direction
            if(loco->numero() == loco1Number && !sens){
                section->acquire();
                section->setaboutToCrossSwitch2(true);
            }
        }
        return pos;
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

    void stop(){
        arreter();
        while(otherLocoHasPriority);
        section->acquire(); // restart when the section is free
        depart();
        section->release();
    }
    // since QList does not seem to have a proper reverse function
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

    void setOtherLocoPriority(bool b){
        otherLocoHasPriority = b;
    }
};

#endif // WORKER_H
