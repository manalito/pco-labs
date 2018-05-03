#include "ctrain_handler.h"
#include "locomotive.h"

#include <QThread>
#include <QList>
#include <QPair>
#include <QSemaphore>
#include <QMutex>


#include "section.h"
#include "worker.h"

#include <QList>

//Creation d'une locomotive
static Locomotive locomotive1;
static Locomotive locomotive2;

/* Les numéros des trains doivent correspondre aux numéros des vraies
 * locomotives dans la maquette, afin que les capteurs reconnaissent
 * bien les loco. Pour la simulation, deux numéros différents suffisent.
 *
 * Utilisé uniquement en lecture
 */
int numTrain1 = 1;
int numTrain2 = 2;

/* La vitesse des locomotives peuvent valoir n'importe quelle valeur
 * dans la simulation (tant que l'inertie est désactivée). Lors de
 * l'utilisation de la maquette, celles-ci ne devraient pas dépasser
 * 10, pour des raisons mécaniques.
 *
 * Utilisé uniquement en lecture
 */
int vitesseLoco1 = 14;
int vitesseLoco2 = 9;

/*
//Arret d'urgence
void emergency_stop()
{
    locomotive.arreter();
    afficher_message("\nSTOP!");
}
*/


//Creation des locomotives
QList<Worker*> locomotives;


//Arret d'urgence
void emergency_stop()
{
    for(Worker* loco : locomotives){
        loco->arreter();
    }
    afficher_message("\nSTOP!");
}

/*
//Fonction principale
int cmain()
{
    afficher_message("Hit play to start the simulation...");

    //Choix de la maquette
    selection_maquette(MAQUETTE_A);

    //Initialisation d'un parcours
    QList<int> parcours;
    parcours << 16 << 15 << 14 << 7 << 6 << 5 << 34 << 33 << 32 << 25 << 24;

    //Initialisation des aiguillages
    diriger_aiguillage(8,  DEVIE,       0);
    diriger_aiguillage(2,  DEVIE,       0);
    diriger_aiguillage(20, DEVIE,       0);
    diriger_aiguillage(14, DEVIE,       0);
    diriger_aiguillage(11, TOUT_DROIT,  0);
    diriger_aiguillage(17, TOUT_DROIT,  0);
    diriger_aiguillage(23, TOUT_DROIT,  0);

    //Initialisation de la locomotive
    locomotive.fixerNumero(1);
    locomotive.fixerVitesse(12);
    locomotive.fixerPosition(16, 23);
    locomotive.allumerPhares();
    locomotive.demarrer();
    locomotive.afficherMessage("Ready!");

    //Attente du passage sur les contacts
    for (int i = 0; i < parcours.size(); i++) {
        attendre_contact(parcours.at(i));
        afficher_message(qPrintable(QString("The engine no. %1 has reached contact no. %2.")
                                    .arg(locomotive.numero()).arg(parcours.at(i))));
        locomotive.afficherMessage(QString("I've reached contact no. %1.").arg(parcours.at(i)));
    }

    //Arreter la locomotive
    locomotive.arreter();
    locomotive.afficherMessage("Yeah, piece of cake!");

    //Fin de la simulation
    mettre_maquette_hors_service();

    //Exemple de commande
    afficher_message("Enter a command in the input field at the top of the window.");
    QString commande = getCommand();
    afficher_message(qPrintable(QString("Your command is: ") + commande));

    return EXIT_SUCCESS;
}
*/

/*
//Fonction principale
int cmain()
{
    afficher_message("Hit play to start the simulation...");

    //Choix de la maquette
    selection_maquette(MAQUETTE_B);

    //Initialisation d'un parcours
    QList<int> parcours;
    parcours << 24 << 21 << 16 << 15 << 10 << 11 << 6 << 5;

    //Initialisation des aiguillages
    diriger_aiguillage(16, TOUT_DROIT,  0);
    diriger_aiguillage(15, DEVIE,       0);
    diriger_aiguillage(12, DEVIE,       0);
    diriger_aiguillage(11, DEVIE,       0);
    diriger_aiguillage(9,  TOUT_DROIT,  0);
    diriger_aiguillage(5,  TOUT_DROIT,  0);
    diriger_aiguillage(8,  DEVIE,       0);
    diriger_aiguillage(7,  TOUT_DROIT,  0);
    diriger_aiguillage(4,  TOUT_DROIT,  0);
    diriger_aiguillage(3,  TOUT_DROIT,  0);

    //Initialisation de la locomotive
    locomotive.fixerNumero(1);
    locomotive.fixerVitesse(12);
    locomotive.fixerPosition(24, 5);
    locomotive.allumerPhares();
    locomotive.start();
    locomotive.afficherMessage("Ready!");

    //Init loco 2
    locomotive2.fixerNumero(2);
    locomotive2.fixerVitesse(16);
    locomotive2.fixerPosition(3, 22);
    locomotive2.allumerPhares();
    locomotive2.start();
    locomotive2.afficherMessage("Ready2!");


    //Attente du passage sur les contacts
    for (int i = 0; i < parcours.size(); i++) {
        attendre_contact(parcours.at(i));
        afficher_message(qPrintable(QString("The engine no. %1 has reached contact no. %2.")
                                    .arg(locomotive.numero()).arg(parcours.at(i))));
        locomotive.afficherMessage(QString("I've reached contact no. %1.").arg(parcours.at(i)));
    }

    //Arreter la locomotive
    locomotive.arreter();
    locomotive.afficherMessage("Yeah, piece of cake!");

    //Arreter la locomotive
    locomotive2.arreter();
    locomotive2.afficherMessage("Yeah, piece of cake!");

    //Fin de la simulation
    mettre_maquette_hors_service();

    //Exemple de commande
    afficher_message("Enter a command in the input field at the top of the window.");
    QString commande = getCommand();
    afficher_message(qPrintable(QString("Your command is: ") + commande));

    return EXIT_SUCCESS;
}

*/

//Fonction principale
int cmain()
{
    afficher_message("Hit play to start the simulation...");

        //Zone critique locomotives
        QList<int> critique1, critique2, critique1deviation;
        critique1 << 13 << 10 << 9 << 4 << 3 << 19;
        critique1deviation << 13 << 10 << 11 << 6 << 3 << 19;
        critique2 << 17 << 12 << 9 << 4 << 5 << 24;

        //Depart locomotives
        QPair<int, int> start1 = QPair<int,int>(14, 19);
        QPair<int, int> start2 = QPair<int,int>(18, 23);

        //Choix de la maquette
        selection_maquette(MAQUETTE_B);

        //Initialisation des parcours
        QList<int> parcours1, parcours1deviation, parcours2;
        parcours1 << 14 << 13 << 10 << 9 << 4 << 3 << 19;
        parcours1deviation << 14 << 13 << 10 << 11 << 6 << 3 << 19;
        parcours2 << 18 << 17 << 12 << 9 << 4 << 5 << 24 << 23;

        //Zone critique partagée
        Section* section = new Section(numTrain1, numTrain2);

        //Initialisation des locomotives
        locomotives.append(new Worker(numTrain1, vitesseLoco1, start1, true,
                                      parcours1, critique1, section,
                                      numTrain1, numTrain2, parcours1deviation,
                                      critique1deviation));
        locomotives.append(new Worker(numTrain2, vitesseLoco2, start2, true,
                                      parcours2, critique2, section,
                                      numTrain1, numTrain2, parcours1deviation,
                                      critique1deviation));

        //Initialisation des aiguillages pour loco 1
        diriger_aiguillage(14, TOUT_DROIT, 0);
        diriger_aiguillage(10, TOUT_DROIT, 0);
        diriger_aiguillage(5,  TOUT_DROIT, 0);
        diriger_aiguillage(9,  DEVIE, 0);
        diriger_aiguillage(1,  TOUT_DROIT, 0);
        diriger_aiguillage(13, DEVIE,  0);

        //Initialisation des aiguillages pour loco 2
        diriger_aiguillage(12, TOUT_DROIT, 0);
        diriger_aiguillage(11, TOUT_DROIT, 0);
        diriger_aiguillage(16, TOUT_DROIT, 0);
        diriger_aiguillage(15, TOUT_DROIT, 0);

        //Lancement de la locomotive
        locomotives.at(0)->start();
        locomotives.at(1)->start();

        locomotives.at(0)->wait();
        locomotives.at(1)->wait();

        //Fin de la simulation
        mettre_maquette_hors_service();

        //Exemple de commande
        afficher_message("Enter a command in the input field at the top of the window.");
        QString commande = getCommand();
        afficher_message(qPrintable(QString("Your command is: ") + commande));

    return EXIT_SUCCESS;
}
