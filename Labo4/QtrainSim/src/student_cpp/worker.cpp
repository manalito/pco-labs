#include "worker.h"

Worker::Worker(const Locomotive &locomotive, const QList<int> &parcours,
                       const QList<Section*>& map, const std::vector<std::pair<int, int>> switchsMap,
                       const bool highPriority)
{
    this->locomotive = locomotive;
    this->parcours = parcours;
    this->map = map;
    this->switchsMap = switchsMap;
    this->highPriority = highPriority;
}

Worker::~Worker()
{

}

void Worker::process() {
    forever {
        for (int nb_tours = 0; nb_tours < 2; nb_tours++) {
            //Attente du passage sur les contacts
            for (int i = 0; i < parcours.size(); i++) {
                attendre_contact(parcours.at(i));

                int contact1 = parcours.at(i);
                int contact2;
                int contact3;
                int contact4;

                contact2 = parcours.at((i + 1) % parcours.size());
                contact3 = parcours.at((i + 2) % parcours.size());
                contact4 = parcours.at((i + 3) % parcours.size());

                for (int j = 0; j < map.size(); j++) {
                    Section* section = map.at(j);

                    if (section->containsContact(contact1, contact2)) {
                        if(currentSection) {
                            currentSection->unlock();
                        }

                        currentSection = section;
                        section->lock(locomotive, switchsMap);
                    } else if (section->containsContact(contact2, contact3)) {
                        section->lock(locomotive, switchsMap);
                    } else if (section->containsContact(contact3, contact4) && highPriority) {
                        section->lock(locomotive, switchsMap, false);
                    }
                }

                afficher_message(qPrintable(QString("The engine no. %1 has reached contact no. %2.").arg(locomotive.numero()).arg(parcours.at(i))));
                locomotive.afficherMessage(QString("I've reached contact no. %1.").arg(parcours.at(i)));
            }
        }

        inverser_sens_loco(locomotive.numero());
        std::reverse(parcours.begin(), parcours.end());
    }

    emit finished();
}
