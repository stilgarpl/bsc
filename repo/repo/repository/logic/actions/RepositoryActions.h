//
// Created by stilgar on 30.01.19.
//

#ifndef BASYCO_REPOACTIONS_H
#define BASYCO_REPOACTIONS_H



struct RepositoryActions {


    //@todo implement
    static bool checkIfUpdateRequired(const JournalPtr &currentJournal, const JournalPtr &newJournal) {
        LOGGER("REPO CHECK IF U")
        if (newJournal == nullptr || currentJournal == nullptr) {
            return false;
        }

        //@todo add more checks
        return newJournal->getChecksum() == currentJournal->getChecksum();

    }

};


#endif //BASYCO_REPOACTIONS_H
