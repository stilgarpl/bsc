//
// Created by stilgar on 30.01.19.
//

#ifndef BASYCO_REPOACTIONS_H
#define BASYCO_REPOACTIONS_H


#include <repo/repository/logic/events/RepositoryEvent.h>

struct RepositoryActions {


    //@todo implement
    static bool checkIfUpdateRequired(const JournalPtr &currentJournal, const JournalPtr &newJournal) {
        LOGGER("REPO CHECK IF U")
        if (newJournal == nullptr) {
            return false;
        }


        return false;
    }

    static void getRepositoryInformation(const RepositoryEvent &repositoryEvent);
};


#endif //BASYCO_REPOACTIONS_H
