//
// Created by stilgar on 05.11.17.
//

#ifndef BSC_REPOMODULE_H
#define BSC_REPOMODULE_H


#include <p2p/core/node/module/NodeModule.h>
#include <repo/repository/RepositoryManager.h>
#include <p2p/core/dependency/DependencyManaged.h>
#include <p2p/modules/network/NetworkModule.h>
#include <repo/journal/network/packet/JournalGroup.h>
#include <repo/repository/storage/StorageManager.h>
#include <repo/repository/storage/StorageFactorySpecialization.h>
namespace bsc {

    class RepoModule : public bsc::NodeModuleDependent<RepoModule> {
    public:
        class Configuration : public bsc::IConfig {
        private:
            PathType repositoryDataPath = fs::path("repository");
            PathType storagePath = fs::path("storage");
            bool autoProcess = true;

        public:
            //@todo make it return reference when cereal path is fixed.
            [[nodiscard]] std::filesystem::path getRepositoryDataPath() const;

            void setRepositoryDataPath(const std::filesystem::path& repositoryDataPath);

            [[nodiscard]] std::filesystem::path getStoragePath() const;

            void setStoragePath(const PathType& storagePath);

        private:
            template<class Archive>
            void serialize(Archive& ar) {
                ar(CEREAL_NVP(repositoryDataPath), CEREAL_NVP(autoProcess), CEREAL_NVP(storagePath));
            }

        public:
            [[nodiscard]] bool isAutoProcess() const;

            void setAutoProcess(bool autoProcess);

        private:


            friend class cereal::access;
        };

    private:
        RepositoryManager repositoryManager;
        StorageManager storageManager;
        RepositoryPtr selectedRepository = nullptr;
    public:
        explicit RepoModule(bsc::INode& node);

        void setupActions(bsc::ILogicModule::SetupActionHelper& actionHelper) override;

        bool assignActions(bsc::ILogicModule::AssignActionHelper& actionHelper) override;

        bool setupSources(bsc::ILogicModule::SetupSourceHelper& sourceHelper) override;

        RepositoryPtr findRepository(const IRepository::RepoIdType& repoId);

        IStoragePtr findStorage(const IStorage::StorageId& storageId);

        void initialize() override;

        ////////////////////////////////
        /// Commands section
        ////////////////////////////////

        void persistFile(const fs::path& path);

        void ignoreFile(const fs::path& path);

        void updateFile(const fs::path& path);

        void updateAllFiles();

        void deployAllFiles();

        void persistFile(const IRepository::RepoIdType& repoId, const fs::path& path);


        void selectRepository(const IRepository::RepoIdType& repoId);

        RepositoryPtr createRepository(const IRepository::RepoIdType& repoId);

        IStoragePtr createStorage(const bsc::Factory<IStoragePtr, bsc::StorageFactoryByType>::SelectorType& storageType,
                                  const IStorage::StorageId& storageId);

        void saveRepository(const IRepository::RepoIdType& repoId);

        void loadRepository(const IRepository::RepoIdType& repoId);

        void restoreRepository(const IRepository::RepoIdType& repoId);

        void printHistory();

        void downloadRemoteRepository(const bsc::NodeIdType& remoteId, const IRepository::RepoIdType& repoId);

        void downloadRepository(const IRepository::RepoIdType& repoId);

        void deployRepository(const IRepository::RepoIdType& repoId);

        void findNewestRepositoryInTheNetwork(const IRepository::RepoIdType& repoId) {
            //broadcast to every node get repo info packet
            //for every response:
            //  check if it has something this node hasn't
            //  if it does, download repo -> update repo
        }


        const RepositoryPtr& getSelectedRepository() const;


        void prepareSubmodules() override;

    };
}

#endif //BSC_REPOMODULE_H
