//
// Created by stilgar on 07.11.17.
//

#include <p2p/filesystem/network/logic/sources/FileSource.h>
#include <p2p/filesystem/network/logic/actions/FileActions.h>
#include <p2p/filesystem/transfer/FileTransferControl.h>
#include "FilesystemModule.h"
#include "BasicModule.h"

FilesystemModule::FilesystemModule(INode &node) : NodeModule(node) {
    setRequired<BasicModule, NodeNetworkModule>();
}

void FilesystemModule::setupActions(LogicManager &logicManager) {
    logicManager.setAction<FileRequestEvent>("fileReq", FileActions::sendFile);
    logicManager.setAction<FileResponseEvent>("fileRes", FileActions::receivedFile);
    logicManager.setAction<FileAttributesEvent>("fileAtrS", FileActions::sendAttributes);
    logicManager.setAction<FileAttributesEvent>("fileAtrR", FileActions::receivedAttributes);

}

bool FilesystemModule::assignActions(LogicManager &logicManager) {
    if (logicManager.assignAction<FileRequestEvent>(FileRequestEvent::IdType::GET_FILE, "fileReq") &&
        logicManager.assignAction<FileRequestEvent>(FileRequestEvent::IdType::GET_CHUNK, "fileReq") &&
        logicManager.assignAction<FileResponseEvent>(FileResponseEvent::IdType::FILE_RECEIVED, "fileRes") &&
        logicManager.assignAction<FileResponseEvent>(FileResponseEvent::IdType::CHUNK_RECEIVED, "fileRes") &&
        logicManager.assignAction<FileAttributesEvent>(FileAttributesEvent::IdType::ATTRIBUTES_REQUESTED, "fileAtrS") &&
        logicManager.assignAction<FileAttributesEvent>(FileAttributesEvent::IdType::ATTRIBUTES_RECEIVED, "fileAtrR")) {
        std::clog << "Debug: File assignment!" << std::endl;


        ///@todo remove this debug:

//        logicManager.setAction<ConnectionEvent>("fileSendDebug", [](const ConnectionEvent &event) {
//
//            SendFile::Request::Ptr req = SendFile::Request::getNew();
//            req->setFilePath("/tmp/basyco/testfile.txt");
//            req->setBegin(3);
//            req->setEnd(8);
//            event.getConnection()->send(req);
//        });
//
//        if (logicManager.assignAction<ConnectionEvent>(ConnectionEvent::IdType::CONNECTION_ESTABLISHED,
//                                                       "fileSendDebug")) {
//            std::clog << "Debug: File DEBUG assignment!" << std::endl;
//        }
        return true;
    }
    return false;
}

bool FilesystemModule::setupSources(LogicManager &logicManager) {
    logicManager.addSource<FileSource>();
    return true;
}

const std::experimental::filesystem::path &FilesystemModule::getCurrentPath() const {
    return currentPath;
}

FileTransferDescriptorPtr FilesystemModule::remoteGetFile(const NodeIdType &nodeId, fs::path from, fs::path to) {

    if (to.is_relative()) {
        to = currentPath / to;
    }
    auto ret = FileTransferControl::initiateTransfer(node, nodeId, from, to);

    currentTransfers.push_back(ret);
    return ret;
}
