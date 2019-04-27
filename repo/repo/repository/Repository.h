#include <utility>

//
// Created by stilgar on 17.10.17.
//

#ifndef BASYCO_REPOSITORY_H
#define BASYCO_REPOSITORY_H


#include <repo/journal/SimpleJournal.h>
#include <p2p/log/Logger.h>
#include <repo/repository/storage/IStorage.h>
#include <repo/repository/storage/InternalStorage.h>
#include <p2p/utils/crypto.h>
#include <repo/repository/transformer/PathTransformer.h>
#include "IRepository.h"

class Repository : public IRepository {

public:

    class RepoFileMap {
    public: //@todo should be private, I think.
        class Attributes {
            fs::perms permissions = fs::perms::none;
            uintmax_t size = 0;
            //@todo utc clock instead of old time_t value. or, typedef clock
            std::time_t modificationTime = 0;
            ChecksumId checksum; //checksum of the file.
            bool directory = false;
            IStorage::ResourceId resourceId;
        public:
            fs::perms getPermissions() const;

            uintmax_t getSize() const;

            time_t getModificationTime() const;

            const ChecksumId &getChecksum() const;

            const IStorage::ResourceId &getResourceId() const;

            Attributes() = default;

            explicit Attributes(const JournalStateData &data);

            bool isDirectory() const;

            FileData toFileData() {
                return FileData(checksum, permissions, size, modificationTime, directory);
            }

            FileData toFileData(const fs::path &path) {
                return FileData(path, checksum, permissions, size, modificationTime, directory);
            }

        };

        class DeleteInfo {
        private:
            bool deleted = false;
            //@todo store utc_clock value instead of old time_t
            std::time_t deletionTime = 0;

        public:
            bool isDeleted() const;

            void setDeleted(bool deleted);

            time_t getDeletionTime() const;

            void setDeletionTime(time_t deletionTime);
        };

    private:
        std::map<fs::path, std::optional<Attributes>> attributesMap;
        std::map<fs::path, DeleteInfo> deleteMap;
        JournalPtr &journal;
        std::shared_ptr<IPathTransformer> &pathTransformer;
        decltype(journal->getChecksum()) mapChecksum;
    private:
        void prepareMap();

    public:
        auto operator[](const fs::path &path) -> decltype(attributesMap[fs::current_path()]);

        auto getSize(const fs::path &path);

        decltype(attributesMap) subMap(const fs::path &root);

    public:

    public:

        auto begin() -> decltype(attributesMap.begin());

        auto end() -> decltype(attributesMap.end());

        std::time_t getDeletionTime(const fs::path &path);

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


            friend class cereal::access;

        };

    public:

        std::map<fs::path, std::optional<DeployAttributes>> deployMap;

        auto begin() -> decltype(deployMap.begin());

        auto end() -> decltype(deployMap.end());

        auto operator[](const fs::path &path) -> decltype(deployMap[fs::current_path()]);

        void markDeployed(const fs::path &path, bool value = true) {
            deployMap[path]->setDeployed(value);
        }

        bool isDeployed(const fs::path &path) {
            return deployMap[path] && deployMap[path]->isDeployed();
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

    enum class RepositoryAction {
        PERSIST,
        UPDATE, //this name is a little ambigious
        DELETE,
        TRASH,
        REMOVE,
        RESTORE,
        NOP,
    };

    class RepositoryActionStrategy {
    protected:
        Repository &repository;
    public:
        bool apply(const fs::path &path) {
            return this->apply(path, std::nullopt);
        }

        //right now it returns deployed state, but maybe it should return enum or sth //@todo think about it
        virtual bool apply(const fs::path &path, const std::optional<RepoFileMap::Attributes> &attributes) = 0;

        explicit RepositoryActionStrategy(Repository &repository) : repository(repository) {}
    };

protected:
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

    //RepositoryActionStrategyPack

    using StrategyType = std::shared_ptr<RepositoryActionStrategy>;
    using RepositoryActionStrategyPack = RepoPack<StrategyType>;
    using RepoActionPack = RepoPack<RepositoryAction>;

    class RepositoryActionStrategyFactory {
    protected:
        Repository &repository;
    public:
        std::shared_ptr<RepositoryActionStrategy> create(RepositoryAction action);

        //maybe should return unique or shared_ptr? maybe not @todo think about it
        RepositoryActionStrategyPack createPack(RepoActionPack actionPack);
        RepositoryActionStrategyFactory(Repository &repository);
    };




private:
    JournalPtr journal = std::make_shared<SimpleJournal>();
    RepositoryActionStrategyFactory strategyFactory;
    RepoIdType repositoryId;
    std::shared_ptr<IStorage> storage;
    std::shared_ptr<IPathTransformer> pathTransformer = std::make_shared<PathTransformer>();
public: //@todo should it be public?
    RepositoryActionStrategyPack deployPack;
    RepositoryActionStrategyPack updatePack;
    RepositoryActionStrategyPack fullPack;


    RepoFileMap _repoFileMap = RepoFileMap(journal, pathTransformer);
    //@todo add saving and loading of a deployMap
    RepoDeployMap deployMap;

protected:

    //@todo move this function inside RepoFileMap. Store more data in RepoFileMap - it should know modification date of each path and type (dir/file)
    RepoFileMap &getFileMap() {
        return _repoFileMap;
    }

public:
    const RepoIdType &getRepositoryId() const override;

    //@todo hide this perhaps?
    JournalPtr &getJournal();

    const std::shared_ptr<IStorage> &getStorage() const;

    void setJournal(const JournalPtr &journal);

    void downloadStorage();

    void restoreAll();

    void commit();

public:

    void persist(fs::path path);

    void forget(fs::path path);

    void remove(fs::path path);

    void ignore(fs::path path);

    void restoreAttributes(const fs::path &path);

    //update one file from the repository
    void update(fs::path path, const RepositoryActionStrategyPack &strategyPack);

    //update everything
    void update();

    explicit Repository(RepoIdType repositoryId);

    void trash(const fs::path &path);

    ~Repository() override = default;
};

typedef std::shared_ptr<Repository> RepositoryPtr;

#endif //BASYCO_REPOSITORY_H
