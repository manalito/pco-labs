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
    Locomotive* locomotive;
    QList<int> parcours;
    QPair<int, int> capteurCritique;
    Section* zoneCritique;
    bool sens; //true = marche avant
    int nbTour; // Utile pour changer de sens après 2 tours
    int numTrain2;
public:

    //Initialisation de la locomotive
    Worker(int id, int vitesse, QPair<int, int> departCapteur,
                     bool phare, QList<int> parcours,
                     QPair<int, int> capteurCritique,
                     Section* zoneCritique, int numTrain2){
        locomotive = new Locomotive();
        locomotive->fixerNumero(id);
        locomotive->fixerVitesse(vitesse);
        locomotive->fixerPosition(departCapteur.first, departCapteur.second);
        if(phare) locomotive->allumerPhares();
        this->parcours = parcours;
        this->capteurCritique = capteurCritique;
        this->zoneCritique = zoneCritique;
        sens = true;
        nbTour = 0;
        this->numTrain2 = numTrain2;
        locomotive->afficherMessage("Ready!");
    }

    void depart(){
        locomotive->demarrer();
        locomotive->afficherMessage(qPrintable(QString("The engine is starting!")));
    }
    void arreter(){
        locomotive->arreter();
        locomotive->afficherMessage(qPrintable(QString("The engine is stopping!")));
    }

    void run() Q_DECL_OVERRIDE{
        depart();

        /* Attente du passage sur les contacts. La zone critique n'a pas de début
         * ou de fin, mais des bornes (utile pour gérer les deux sens).
         *
         */
        int pos = 0;
        while(true){
            contact(parcours.at(pos));
            // Action à l'entrée de la zone critique
            if(parcours.at(pos) == capteurCritique.first ||
                    parcours.at(pos) == capteurCritique.second){
                // Action spéciale pour la loco 2 si la zone est occupée:
                // elle s'arrête et attend que l'autre sorte de la zone
               if(!zoneCritique->peutEntrer(locomotive->numero()) &&
                       locomotive->numero() == numTrain2){
                   arreter();
                   zoneCritique->bloquer();
                   // Avant de repartir, la loco 2 indique qu'elle entre dans la zone critique
                   zoneCritique->setLibre(false);
                   depart();
               }
               while(true){
                   pos = prochainePosition(pos);
                   if(parcours.at(pos) == capteurCritique.first ||
                           parcours.at(pos) == capteurCritique.second) break;
               }
               contact(parcours.at(pos));
               zoneCritique->sortir(locomotive->numero());
            }
            pos = prochainePosition(pos);
        }
        locomotive->afficherMessage(QString("Yeah, piece of cake for locomotive %1 !")
                                    .arg(locomotive->numero()));
        arreter();
    }

    int prochainePosition(int pos){
        if(sens) pos++;
        else pos--;
        if(pos >= parcours.size()){
           pos = 0;
           nbTour++;
        }
        if(pos < 0){
           pos = parcours.size() - 1;
           nbTour++;
        }
        if(nbTour == 2){
            sens = !sens;
            nbTour = 0;
            // On saute un capteur lors du changement de sens, pour ne pas rester bloqué
            pos = (sens ? 1 : parcours.size() - 2);
            locomotive->inverserSens();
        }

        return pos;
    }

    void contact(int capteur){
        attendre_contact(capteur);
        afficher_message(qPrintable(QString("The engine no. %1 has reached contact no. %2.")
                                    .arg(locomotive->numero()).arg(capteur)));
        locomotive->afficherMessage(QString("I've reached contact no. %1.").arg(capteur));
    }
};

#endif // WORKER_H
