#include <utility>

//
// Created by stilgar on 17.10.17.
//

#ifndef BSC_REPOSITORY_H
#define BSC_REPOSITORY_H


#include "IRepository.h"
#include "RepositoryAttributes.h"
#include "RepositoryFileMap.h"
#include "RepositoryManipulator.h"
#include <core/factory/FactoryContext.h>
#include <core/log/Logger.h>
#include <core/utils/crypto.h>
#include <repo/journal/SimpleJournal.h>
#include <repo/repository/storage/IStorage.h>
#include <repo/repository/storage/StorageFactorySpecialization.h>
#include <repo/repository/transformer/PathTransformer.h>
namespace bsc {

    class Repository : public IRepository {
    public:
        const IPathTransformer& getPathTransformer() override;

    public:
        class RepoDeployMap {

            class DeployAttributes {
                bool deployed = false;

            public:
                bool isDeployed() const;

                void setDeployed(bool deployed);

            private:
                template<class Archive>
                void serialize(Archive& ar) {
                    ar(deployed);
                }

            public:
                DeployAttributes(bool deployed);

                DeployAttributes();


                friend class cereal::access;

            };

        public:
            std::map<fs::path, std::optional<DeployAttributes>> deployMap;
//            std::map<std::string, std::optional<DeployAttributes>> deployMap;

            auto begin() -> decltype(deployMap.begin());

            auto end() -> decltype(deployMap.end());

            auto operator[](const fs::path& path) -> decltype(deployMap[fs::current_path()]);

            void markDeployed(const fs::path& path, DeployState deployState);

            bool isDeployed(const fs::path& path) {
                return deployMap[path] && deployMap[path]->isDeployed();
            }

        private:
            template<class Archive>
            void serialize(Archive& ar) {
                ar(deployMap);
            }


            friend class cereal::access;

        };


    public:
        //@todo this can probably be extracted as a separate class and all predefined packs can be made const,static and public to be able to be shared
        class RepositoryActionStrategyFactory {

        public:
            [[nodiscard]] std::shared_ptr<RepositoryActionStrategy> create(RepositoryAction action) const;

            //maybe should return unique or shared_ptr? maybe not @todo think about it
            [[nodiscard]] RepositoryActionStrategyPack createPack(RepoActionPack actionPack) const;

        };


    private:
        JournalPtr journal = std::make_shared<SimpleJournal>();
        RepositoryManipulator manipulator;
        const RepositoryActionStrategyFactory strategyFactory{};
        const RepoIdType repositoryId;
        std::shared_ptr<IStorage> storage;
        std::shared_ptr<IPathTransformer> pathTransformer = std::make_shared<PathTransformer>();

    private:
        const RepositoryActionStrategyPack deployPack;
        const RepositoryActionStrategyPack localSyncPack;
        const RepositoryActionStrategyPack fullPack;

    private:
        RepoDeployMap deployMap;
        RepositoryFileMapRenderer fileMapRenderer;

    public:
        Repository(const IRepository::RepoIdType& repositoryId, const std::shared_ptr<IStorage>& storage,
                   const JournalPtr& journal, const Repository::RepoDeployMap& deployMap);

    protected:

    public:
        const RepoIdType& getRepositoryId() const override;

        //@todo hide this perhaps?
        JournalPtr& getJournal() override;

        const std::shared_ptr<IStorage>& getStorage() const;

        void setJournal(const JournalPtr& journal) override;

        void downloadStorage() override;

        void restoreAll() override;

        void commit() override;

    public:

        //update one file from the repository
        void update(fs::path path, const RepositoryActionStrategyPack& strategyPack,
                    std::set<UpdateOptions> updateOptions) override;

        //update everything
        void syncLocalChanges() override;

        //deploy everything, apply repository to filesystem @todo add force levels, what to do with changed files. standard = just create files that are not there, force = replace changed files, muchForce = replace everything.
        void deploy() override;

        explicit Repository(RepoIdType repositoryId, IStoragePtr storagePtr);

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
        void serialize(Archive& ar) {
            ar(cereal::base_class<IRepository>(this), repositoryId, journal, deployMap, storage->getStorageId());
        }

        template<class Archive>
        static void load_and_construct(Archive& ar, cereal::construct<Repository>& construct) {
            Repository::RepoIdType r;
            JournalPtr j;
            RepoDeployMap d;
            IStorage::StorageId storageId;
            ar(r, j, d, storageId);
            auto factoryContext = Context::getActiveContext()->get<FactoryContext>();
            auto storageFactory = factoryContext->getFactory<IStoragePtr, bsc::StorageFactoryByName>();
            auto storagePtr = storageFactory->create(storageId);
            construct(r, storagePtr, j, d);
        }


    public:
        friend class cereal::access;

        [[nodiscard]] const RepositoryActionStrategyPack& getDeployPack() const override;

        [[nodiscard]] const RepositoryActionStrategyPack& getLocalSyncPack() const override;

        [[nodiscard]] const RepositoryActionStrategyPack& getFullPack() const override;
    };
}
CEREAL_REGISTER_TYPE(bsc::Repository)
CEREAL_REGISTER_POLYMORPHIC_RELATION(bsc::IRepository, bsc::Repository)

#endif //BSC_REPOSITORY_H
