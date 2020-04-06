//
// Created by stilgar on 22.12.17.
//

#ifndef BSC_IREPOSITORY_H
#define BSC_IREPOSITORY_H


#include "RepositoryAttributes.h"
#include <memory>
#include <repo/journal/Journal.h>
#include <repo/repository/repository/DeployState.h>
#include <repo/repository/repository/RepositoryAction.h>
#include <repo/repository/repository/RepositoryActionStrategy.h>

namespace bsc {
    class IStorage;

    class IRepository {
    public:
        typedef std::string RepoIdType;

        virtual const RepoIdType& getRepositoryId() const = 0;

        virtual ~IRepository() = default;

        //@todo hide this perhaps?
        virtual JournalPtr& getJournal() = 0;

        virtual void downloadStorage() = 0;

        //everything journal -> filesystem
        virtual void restoreAll() = 0;
        virtual void commit() = 0;
        virtual void setJournal(const JournalPtr& journal) = 0;
        virtual const std::shared_ptr<IStorage>& getStorage() const = 0;
        virtual const PathTransformer& getPathTransformer() = 0;

        //everything filesystem -> journal
        virtual void syncLocalChanges() = 0;

        //deploy everything, apply repository to filesystem @todo add force levels, what to do with changed files. standard = just create files that are not there, force = replace changed files, muchForce = replace everything.
        virtual void deploy() = 0;

    private:
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
            followDirectories,
            followUpdatedDirectories,
        };

        //RepositoryActionStrategyPack

        using StrategyType = std::shared_ptr<RepositoryActionStrategy>;
        using RepositoryActionStrategyPack = RepoPack<StrategyType>;
        using RepoActionPack = RepoPack<RepositoryAction>;

//update one file from the repository
        virtual void update(fs::path path, const RepositoryActionStrategyPack& strategyPack,
                            std::set<UpdateOptions> updateOptions) = 0;

        [[nodiscard]] virtual const RepositoryActionStrategyPack& getDeployPack() const = 0;

        [[nodiscard]] virtual const RepositoryActionStrategyPack& getLocalSyncPack() const = 0;

        [[nodiscard]] virtual const RepositoryActionStrategyPack& getFullPack() const = 0;

    private:
        template<class Archive>
        void serialize(Archive& ar) {
        }


        friend class cereal::access;

    };

    typedef std::shared_ptr<IRepository> RepositoryPtr;
}
CEREAL_REGISTER_TYPE(bsc::IRepository)

#endif //BSC_IREPOSITORY_H
