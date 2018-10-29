//
// Created by stilgar on 07.11.17.
//

#ifndef BASYCO_FILESYSTEMMODULE_H
#define BASYCO_FILESYSTEMMODULE_H


#include <p2p/node/NodeModule.h>
#include <p2p/dependency/DependencyManaged.h>

#include <filesystem>
#include <p2p/modules/filesystem/transfer/FileTransferDescriptor.h>
#include <p2p/modules/filesystem/network/logic/actions/TransferManager.h>
#include <p2p/modules/filesystem/identification/SimplePathRI.h>

#include "p2p/modules/nodeNetworkModule/NodeNetworkModule.h"

namespace fs = std::filesystem;

class FilesystemModule : public NodeModuleDependent<FilesystemModule> {

    fs::path currentPath = fs::current_path();
    std::list<FileTransferDescriptorPtr> currentTransfers;

    TransferManager transferManager;

public:
    FilesystemModule(INode &node);

    void setupActions(ILogicModule::SetupActionHelper &actionHelper) override;

    bool assignActions(ILogicModule::AssignActionHelper &actionHelper) override;

    bool setupSources(ILogicModule::SetupSourceHelper &sourceHelper) override;

    const std::filesystem::path &getCurrentPath() const;

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
            currentPath += fs::path::preferred_separator + change;
        }

        if (!fs::exists(currentPath)) {
            currentPath = prev;
        }

        currentPath = fs::canonical(currentPath);

    }

    void printWorkingDirectory() {
        std::cout << currentPath << std::endl;
    }

    void listCurrentDirectory() {
        for (auto &&item : fs::directory_iterator(currentPath)) {
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

    auto remoteGetFile(const NodeIdType &nodeId, fs::path from, fs::path to) {
        return transferManager.initiateTransfer(nodeId, std::make_shared<SimplePathRI>(from),
                                                std::make_shared<SimplePathRI>(to));
    }

    auto remoteGetStream(const NodeIdType &nodeId, ResourceIdentificatorPtr from, ResourceIdentificatorPtr to) {
        return transferManager.initiateTransfer(nodeId, from, to);
    }


    void printCurrentTransfers() {
        std::cout << "Current transfers" << std::endl;
        for (auto &&item : currentTransfers) {
            std::cout << "Transfer from : " << item->getSourcePath() << " " << item->getDestinationPath() << std::endl;
            switch (item->getStatus()) {
                case FileTransferStatus::NOT_STARTED :
                    std::cout << "NOT STARTED" << std::endl;

                    break;
                case FileTransferStatus::STARTED:
                    std::cout << "STARTED" << std::endl;
                    break;
                case FileTransferStatus::ATTRIBUTES_ACCQUIRED:
                    std::cout << "ATTRIBUTES_ACCQUIRED" << std::endl;
                    break;
                case FileTransferStatus::DOWNLOADING:
                    std::cout << "DOWNLOADING" << std::endl;
                    break;
                case FileTransferStatus::FINISHED:
                    std::cout << "FINISHED" << std::endl;
                    break;
                case FileTransferStatus::ERROR:
                    std::cout << "ERROR" << std::endl;
                    break;
            }

            std::cout << "Size "
                      << std::to_string(item->getTransferredSize()) + " / " + std::to_string(item->getFileSize())
                      << " (" << std::to_string(100 * item->getTransferredSize() / item->getFileSize()) << "%)"
                      << std::endl;
        }
    }


    void beginTransferTest(const NodeIdType &nodeId, fs::path from) {
        BeginTransfer::Request::Ptr req = BeginTransfer::Request::getNew();
        req->setResourceId(std::make_shared<SimplePathRI>(from));
        auto res = node.getModule<NodeNetworkModule>()->sendPacketToNode(nodeId, req);
        LOGGER("received begin " + std::to_string(res->getTransferId()))
    }
};


#endif //BASYCO_FILESYSTEMMODULE_H
