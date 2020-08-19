#include <utility>

#include <utility>

//
// Created by Krzysztof Tulidowicz on 07.11.17.
//

#ifndef BSC_FILESYSTEMMODULE_H
#define BSC_FILESYSTEMMODULE_H


#include <p2p/core/node/module/NodeModule.h>
#include <p2p/core/dependency/DependencyManaged.h>

#include <filesystem>
#include <p2p/modules/filesystem/transfer/TransferManager.h>
#include <p2p/modules/filesystem/identification/SimplePathRI.h>

#include "p2p/modules/network/NetworkModule.h"

namespace bsc {

    namespace fs = std::filesystem;

    class FilesystemModule : public NodeModuleDependent<FilesystemModule, NetworkModule> {
    public:

        class Configuration {
        public:
            unsigned int maxChunkSize = 20000;

        private:
            template<class Archive>
            void serialize(Archive& ar) {
                ar(maxChunkSize);
            }


            friend class cereal::access;

        };

    private:

        fs::path currentPath = fs::current_path();
        std::list<std::shared_ptr<TransferManager::LocalTransferDescriptor>> currentTransfers;

        TransferManager transferManager;

    public:
        explicit FilesystemModule(INode& node);

        void setupActions(ILogicModule::SetupActionHelper& actionHelper) override;

        bool assignActions(ILogicModule::AssignActionHelper& actionHelper) override;

        bool setupSources(ILogicModule::SetupSourceHelper& sourceHelper) override;

        const std::filesystem::path& getCurrentPath() const;

        void prepareSubmodules() override;


    public:
        ////////////////////////////////
        /// Commands section
        ////////////////////////////////

        void changeDirectory(std::string change) {
            fs::path newPath = change;
            fs::path prev = currentPath;
            if (newPath.is_absolute()) {
                currentPath = std::move(newPath);
            } else {
                currentPath /= change;
            }

            if (!fs::exists(currentPath)) {
                currentPath = prev;
            }

            currentPath = fs::canonical(currentPath);
            fs::current_path(currentPath);

        }

        void printWorkingDirectory() {
            std::cout << currentPath << std::endl;
        }

        void listCurrentDirectory() {
            for (auto&& item : fs::directory_iterator(currentPath)) {
                std::cout << item << std::endl;
            }
        }


        /**
         * @todo possible upgrades
         * this method could return download descriptor
         * with file size
         * downloaded size
         * status - download started, finished, error occurred etc.
         * wait() function to finish downloading
         * stop() function to interrupt download
         *
         * if the file is big enough it should download it in chunks in other thread.
         *
         *
         */
//    FileTransferDescriptorPtr remoteGetFile(const NodeIdType &nodeId, fs::path from, fs::path to);

        auto remoteGetFile(const NodeIdType& nodeId, fs::path from, fs::path to) {
            return transferManager.initiateTransfer(nodeId, std::make_shared<SimplePathRI>(from),
                                                    std::make_shared<SimplePathRI>(to));
        }

        auto remoteGetStream(const NodeIdType& nodeId, ResourceIdentificatorPtr from, ResourceIdentificatorPtr to) {
            return transferManager.initiateTransfer(nodeId, std::move(from), std::move(to));
        }

        //@todo name of this method does not indicate that it creates stuff.
        auto transferQueue() {
            return transferManager.transferQueue();
        }


//    void printCurrentTransfers() {
//        std::cout << "Current transfers" << std::endl;
//        for (auto &&item : currentTransfers) {
//            std::cout << "Transfer from : " << item->getSource() << " " << item->getDestinationPath() << std::endl;
//            switch (item->getStatus()) {
//                case FileTransferStatus::NOT_STARTED :
//                    std::cout << "NOT STARTED" << std::endl;
//
//                    break;
//                case FileTransferStatus::STARTED:
//                    std::cout << "STARTED" << std::endl;
//                    break;
//                case FileTransferStatus::ATTRIBUTES_ACCQUIRED:
//                    std::cout << "ATTRIBUTES_ACCQUIRED" << std::endl;
//                    break;
//                case FileTransferStatus::DOWNLOADING:
//                    std::cout << "DOWNLOADING" << std::endl;
//                    break;
//                case FileTransferStatus::FINISHED:
//                    std::cout << "FINISHED" << std::endl;
//                    break;
//                case FileTransferStatus::error:
//                    std::cout << "error" << std::endl;
//                    break;
//            }
//
//            std::cout << "Size "
//                      << std::to_string(item->getTransferredSize()) + " / " + std::to_string(item->getFileSize())
//                      << " (" << std::to_string(100 * item->getTransferredSize() / item->getFileSize()) << "%)"
//                      << std::endl;
//        }
//    }
//
//
//    void beginTransferTest(const NodeIdType &nodeId, fs::path from) {
//        BeginTransfer::Request::Ptr req = BeginTransfer::Request::getNew();
//        req->setResourceId(std::make_shared<SimplePathRI>(from));
//        auto res = node.getModule<NetworkModule>()->sendPacketToNode(nodeId, req);
//        LOGGER("received begin " + std::to_string(res->getTransferId()))
//    }
    };
}

#endif //BSC_FILESYSTEMMODULE_H