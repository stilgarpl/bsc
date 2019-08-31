//
// Created by stilgar on 22.12.17.
//

#ifndef BASYCO_IREPOSITORY_H
#define BASYCO_IREPOSITORY_H


#include <memory>
#include <repo/journal/IJournal.h>
#include "RepositoryAttributes.h"

class IStorage;

class IRepository {
public:
    typedef std::string RepoIdType;

    virtual const RepoIdType &getRepositoryId() const =0;
    virtual ~IRepository() = default;
    //@todo hide this perhaps?
    virtual JournalPtr &getJournal()=0;
    virtual void downloadStorage()=0;
    virtual void restoreAll()=0;
    virtual void commit()=0;
    virtual void setJournal(const JournalPtr &journal)=0;
    virtual const std::shared_ptr<IStorage> &getStorage() const=0;

    //@todo direct actions should be probably moved to separate class
    //direct actions:
    virtual void persist(fs::path path)=0;
    virtual void forget(fs::path path)=0;
    virtual void remove(fs::path path)=0;
    virtual void ignore(fs::path path)=0;
    virtual void restoreAttributes(const fs::path &path)=0;
    virtual void trash(const fs::path &path)=0;

    //update everything
    virtual void syncLocalChanges()=0;
    //deploy everything, apply repository to filesystem @todo add force levels, what to do with changed files. standard = just create files that are not there, force = replace changed files, muchForce = replace everything.
    virtual void deploy() =0;

protected:
public:
    enum class DeployState {
        DEPLOYED,
        NOT_DEPLOYED,
        UNCHANGED,
    };

    enum class RepositoryAction {
        PERSIST,
        UPDATE, //this name is a little ambigious
        DELETE,
        TRASH,
        REMOVE,
        RESTORE,
        NOP,
    };
public:
    class RepositoryActionStrategy {
    protected:
        //@todo maybe I should remove repository refefence? strategy packs could then be global and constant. on the other hand, repository would have to be passed everytime to strategy, clouding the interface
        IRepository &repository;
    public:
        DeployState apply(const fs::path &path) {
            return this->apply(path, std::nullopt);
        }

        //right now it returns deployed state, but maybe it should return enum or sth //@todo think about it
        virtual DeployState apply(const fs::path &path, const std::optional<RepositoryAttributes> &attributes) = 0;

        explicit RepositoryActionStrategy(IRepository &repository) : repository(repository) {}
    };

    template<typename PackType>
    struct RepoPack {


        PackType updatedInRepo;
        PackType updatedInFilesystem;
        PackType same;
        PackType newInRepo;
        PackType newInFilesystem;
        PackType deletedInRepo;
        PackType deletedInFilesystem;
    };


public:

    enum class UpdateOptions {
        FOLLOW_DIRECTORIES,
        FOLLOW_UPDATED_DIRECTORIES,
    };

    //RepositoryActionStrategyPack

    using StrategyType = std::shared_ptr<RepositoryActionStrategy>;
    using RepositoryActionStrategyPack = RepoPack<StrategyType>;
    using RepoActionPack = RepoPack<RepositoryAction>;
//update one file from the repository
    virtual void update(fs::path path, const RepositoryActionStrategyPack &strategyPack, std::set<UpdateOptions> updateOptions)=0;

    [[nodiscard]] virtual const RepositoryActionStrategyPack &getDeployPack() const=0;
    [[nodiscard]] virtual const RepositoryActionStrategyPack &getLocalSyncPack() const=0;
    [[nodiscard]] virtual const RepositoryActionStrategyPack &getFullPack() const=0;

private:
    template<class Archive>
    void serialize(Archive &ar) {
    }


    friend class cereal::access;

};

typedef std::shared_ptr<IRepository> RepositoryPtr;

CEREAL_REGISTER_TYPE(IRepository)

#endif //BASYCO_IREPOSITORY_H
