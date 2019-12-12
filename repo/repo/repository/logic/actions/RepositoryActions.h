//
// Created by stilgar on 30.01.19.
//

#ifndef BASYCO_REPOSITORYACTIONS_H
#define BASYCO_REPOSITORYACTIONS_H


struct RepositoryActions {

//@todo those actions should return stuff, journal or repoPtr, to make things simpler and to use full potential of chaining.
    static bool checkIfUpdateRequired(const JournalPtr& currentJournal, const JournalPtr& newJournal);

    static void updateJournal(const IRepository::RepoIdType& repoId, const JournalPtr& newJournal);

    static void downloadRepository(const IRepository::RepoIdType &repoId);

    static void deployRepository(const IRepository::RepoIdType &repoId);

    static void syncRepository(const IRepository::RepoIdType &repoId);

    static void saveRepository(const IRepository::RepoIdType &repoId);

};


#endif //BASYCO_REPOSITORYACTIONS_H
