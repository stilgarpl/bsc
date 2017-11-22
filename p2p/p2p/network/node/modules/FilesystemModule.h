//
// Created by stilgar on 07.11.17.
//

#ifndef BASYCO_FILESYSTEMMODULE_H
#define BASYCO_FILESYSTEMMODULE_H


#include <p2p/network/node/NodeModule.h>
#include <p2p/dependency/DependencyManaged.h>

#include <experimental/filesystem>
#include "NodeNetworkModule.h"

namespace fs = std::experimental::filesystem;

class FilesystemModule : public NodeModule, public DependencyManaged<FilesystemModule> {

    fs::path currentPath = ".";

public:
    FilesystemModule(INode &node);

    void setupActions(LogicManager &logicManager) override;

    bool assignActions(LogicManager &logicManager) override;

    bool setupSources(LogicManager &logicManager) override;


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

    void remoteGetFile(const NodeIdType &nodeId, fs::path from, fs::path to) {
        SendFile::Request::Ptr sendFileRequest = SendFile::Request::getNew();
        // SendFile::Request* sendFileRequest;
        sendFileRequest->setFilePath(from);
        auto netnode = node.getModule<NodeNetworkModule>();
        auto response = netnode->sendPacketToNode(nodeId, sendFileRequest);

        // SendFile::Response* response;
        response->setFilePath(to);
        response->save_file();
    }

};


#endif //BASYCO_FILESYSTEMMODULE_H
