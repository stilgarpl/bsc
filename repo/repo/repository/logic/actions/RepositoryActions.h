//
// Created by stilgar on 30.01.19.
//

#ifndef BASYCO_REPOACTIONS_H
#define BASYCO_REPOACTIONS_H



struct RepositoryActions {


    //@todo implement
    static bool checkIfUpdateRequired(const JournalPtr &currentJournal, const JournalPtr &newJournal);

    static void downloadRepository(const IRepository::RepoIdType repoId);

};


#endif //BASYCO_REPOACTIONS_H
