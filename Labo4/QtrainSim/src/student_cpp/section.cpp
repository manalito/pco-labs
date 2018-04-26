 #include "section.h"

Section::Section()
{

}

Section::Section(QList<int> contacts, QList<int> switchs) {
    this->_contacts = contacts;
    this->switchs = switchs;
    this->isLockBy = 0;
}


Section::~Section(){}


Section::Section(const Section&) {}


QList<int> Section::contacts(){
    return _contacts;
}

void Section::lock(Locomotive& loco, const std::vector<std::pair<int, int>>& switchsMap, const bool blocking) {
    this->mutex.lock();

    if (!isLockBy) {
        this->isLockBy = loco.numero();
        afficher_message("free, locking for:");
        afficher_message(std::to_string(isLockBy).c_str());

        for (unsigned int i = 0; i < switchsMap.size(); i++) {
            for (int j = 0; j < switchs.size(); j++) {
                if (switchsMap.at(i).first == switchs.at(j)) {
                    diriger_aiguillage(switchsMap.at(i).first, switchsMap.at(i).second, 0);
                }
            }
        }

    } else if(isLockBy != loco.numero()) {
        afficher_message("Already locked by:");
        afficher_message(std::to_string(isLockBy).c_str());

        if (blocking) {
           loco.arreter();
        }

        waitingLocomotive = &loco;
        waitingLocomotiveSwitchs = switchsMap;
    }

    mutex.unlock();
}

void Section::unlock() {
    mutex.lock();

    if (waitingLocomotive) {
        for (unsigned int i = 0; i < waitingLocomotiveSwitchs.size(); i++) {
            for (int j = 0; j < switchs.size(); j++) {
                if (waitingLocomotiveSwitchs.at(i).first == switchs.at(j)) {
                    diriger_aiguillage(waitingLocomotiveSwitchs.at(i).first, waitingLocomotiveSwitchs.at(i).second, 0);
                }
            }
        }

        // Débloquer une tâche en attente dans la file
        isLockBy = waitingLocomotive->numero();
        waitingLocomotive->demarrer();
        waitingLocomotive = nullptr;
    } else {
        isLockBy = 0;
    }

    mutex.unlock();
}

bool Section::containsContact(const int contact) const {
    for (int i = 0; i < _contacts.size(); i++) {
        if (_contacts[i] == contact) {
            return true;
        }
    }

    return false;
}

bool Section::containsContact(const int contact1, const int contact2) const {
    int n = 0;

    for (int i = 0; i < _contacts.size(); i++) {
        if (_contacts[i] == contact1 || _contacts[i] == contact2) {
            n++;
            if (n == 2) {
                return true;
            }
        }
    }

    return false;
}
