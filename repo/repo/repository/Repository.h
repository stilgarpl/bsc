#include <utility>

//
// Created by stilgar on 17.10.17.
//

#ifndef BASYCO_REPOSITORY_H
#define BASYCO_REPOSITORY_H


#include <repo/journal/SimpleJournal.h>
#include <core/log/Logger.h>
#include <repo/repository/storage/IStorage.h>
#include <core/utils/crypto.h>
#include <repo/repository/transformer/PathTransformer.h>
#include <core/factory/FactoryContext.h>
#include <repo/repository/storage/StorageFactorySpecialization.h>
#include "IRepository.h"
#include "RepositoryAttributes.h"


class Repository : public IRepository {

public:

    class RepoFileMap {
    public:


        class DeleteInfo {
        private:
            bool deleted = false;
            //@todo store utc_clock value instead of old time_t
            fs::file_time_type deletionTime = fs::file_time_type::min();

        public:
            bool isDeleted() const;

            void setDeleted(bool deleted);

            fs::file_time_type getDeletionTime() const;

            void setDeletionTime(fs::file_time_type deletionTime);
        };

    private:
        std::map<fs::path, std::optional<RepositoryAttributes>> attributesMap;
        std::map<fs::path, DeleteInfo> deleteMap;
        JournalPtr &journal;
        std::shared_ptr<IPathTransformer> &pathTransformer;
        decltype(journal->getChecksum()) mapChecksum;
    private:
        void prepareMap();

    public:
        auto operator[](const fs::path &path) -> decltype(attributesMap[fs::current_path()]);

        auto contains(const fs::path &path) {
            return attributesMap.contains(path);
        }

        auto getSize(const fs::path &path);

        decltype(attributesMap) subMap(const fs::path &root);

    public:

    public:

        auto begin() -> decltype(attributesMap.begin());

        auto end() -> decltype(attributesMap.end());

        fs::file_time_type getDeletionTime(const fs::path &path);

        auto isDeleted(const fs::path &path) -> decltype(deleteMap[path].isDeleted());

        RepoFileMap(JournalPtr &journal, std::shared_ptr<IPathTransformer> &pathTransformer);
    };




    class RepoDeployMap {

        class DeployAttributes {
            bool deployed = false;

        public:
            bool isDeployed() const;

            void setDeployed(bool deployed);

        private:
            template<class Archive>
            void serialize(Archive &ar) {
                ar(deployed);
            }

        public:
            DeployAttributes(bool deployed);

            DeployAttributes();



            friend class cereal::access;

        };

    public:
        //@todo C++17 cereal:
//            std::map<fs::path, std::optional<DeployAttributes>> deployMap;
        std::map<std::string, std::optional<DeployAttributes>> deployMap;
        auto begin() -> decltype(deployMap.begin());

        auto end() -> decltype(deployMap.end());

        auto operator[](const fs::path& path) -> decltype(deployMap[fs::current_path().string()]);

        void markDeployed(const fs::path &path, DeployState deployState);

        bool isDeployed(const fs::path &path) {
            return deployMap[path.string()] && deployMap[path.string()]->isDeployed();
        }

    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar(deployMap);
        }


        friend class cereal::access;

    };

//    enum class RepositorySituation {
//        UPDATED_IN_REPO,
//        UPDATED_IN_FILESYSTEM,
//        SAME,
//        NEW_IN_REPO,
//        NEW_IN_FILESYSTEM,
//        DELETED_IN_REPO,
//        DELETED_IN_FILESYSTEM,
//    };




public:

    class RepositoryActionStrategyFactory {
    protected:
        Repository &repository;
    public:
        [[nodiscard]] std::shared_ptr<RepositoryActionStrategy> create(RepositoryAction action) const;

        //maybe should return unique or shared_ptr? maybe not @todo think about it
        [[nodiscard]] RepositoryActionStrategyPack createPack(RepoActionPack actionPack) const;

        explicit RepositoryActionStrategyFactory(Repository &repository);
    };




private:
    JournalPtr journal = std::make_shared<SimpleJournal>();
    const RepositoryActionStrategyFactory strategyFactory;
    const RepoIdType repositoryId;
    std::shared_ptr<IStorage> storage;
    std::shared_ptr<IPathTransformer> pathTransformer = std::make_shared<PathTransformer>();
public: //@todo should it be public?
    const RepositoryActionStrategyPack deployPack;
    const RepositoryActionStrategyPack localSyncPack;
    const RepositoryActionStrategyPack fullPack;

private:
    RepoFileMap _repoFileMap = RepoFileMap(journal, pathTransformer);
    //@todo add saving and loading of a deployMap
    RepoDeployMap deployMap;
public:
    Repository(const IRepository::RepoIdType &repositoryId, const std::shared_ptr<IStorage> &storage,
               const JournalPtr &journal, const Repository::RepoDeployMap &deployMap);

protected:

    //@todo move this function inside RepoFileMap. Store more data in RepoFileMap - it should know modification date of each path and type (dir/file)
    RepoFileMap &getFileMap() {
        return _repoFileMap;
    }

public:
    const RepoIdType &getRepositoryId() const override;

    //@todo hide this perhaps?
    JournalPtr &getJournal() override ;

    const std::shared_ptr<IStorage> &getStorage() const;

    void setJournal(const JournalPtr &journal) override ;

    void downloadStorage() override ;

    void restoreAll() override ;

    void commit() override ;

public:

    //@todo direct actions should be probably moved to separate class
    //direct actions:
    void persist(fs::path path) override;

    void forget(fs::path path) override;

    void remove(fs::path path) override;

    void ignore(fs::path path) override;

    void restoreAttributes(const fs::path &path) override;

    //update one file from the repository
    void update(fs::path path, const RepositoryActionStrategyPack &strategyPack, std::set<UpdateOptions> updateOptions) override;

    //update everything
    void syncLocalChanges() override;

    //deploy everything, apply repository to filesystem @todo add force levels, what to do with changed files. standard = just create files that are not there, force = replace changed files, muchForce = replace everything.
    void deploy() override ;

    explicit Repository(RepoIdType repositoryId, IStoragePtr storagePtr);


    void trash(const fs::path &path) override;

    ~Repository() override = default;

private:
    //Repository();
//    template<class Archive>
//    void save(Archive &archive) const {
//        archive(repositoryId, journal, deployMap);
//    }
//
//    template<class Archive>
//    void load(Archive &archive) {
//        archive(repositoryId, journal, deployMap);
//    }

private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar(cereal::base_class<IRepository>(this), repositoryId, journal, deployMap, storage->getStorageId());
    }

    template<class Archive>
    static void load_and_construct(Archive &ar, cereal::construct<Repository> &construct) {
        Repository::RepoIdType r;
        JournalPtr j;
        RepoDeployMap d;
        IStorage::StorageId storageId;
        ar(r, j, d, storageId);
        auto& factoryContext = bsc::Context::getActiveContext()->get<bsc::FactoryContext>();
        auto storageFactory = factoryContext.getFactory<IStoragePtr, bsc::StorageFactoryByName>();
        auto storagePtr = storageFactory->create(storageId);
        construct(r, storagePtr, j, d);
    }



public:
    friend class cereal::access;
    [[nodiscard]] const RepositoryActionStrategyPack &getDeployPack() const;
    [[nodiscard]] const RepositoryActionStrategyPack &getLocalSyncPack() const;
    [[nodiscard]] const RepositoryActionStrategyPack &getFullPack() const;


};

CEREAL_REGISTER_TYPE(Repository)
CEREAL_REGISTER_POLYMORPHIC_RELATION(IRepository, Repository)

#endif //BASYCO_REPOSITORY_H
