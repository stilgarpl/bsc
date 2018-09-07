//
// Created by stilgar on 07.11.17.
//

#include <p2p/modules/filesystem/network/logic/sources/FileSource.h>
#include <p2p/modules/filesystem/network/logic/actions/FileActions.h>
#include <p2p/modules/filesystem/transfer/FileTransferControl.h>
#include <p2p/modules/filesystem/network/logic/sources/TransferSource.h>
#include "FilesystemModule.h"
#include "p2p/modules/basic/BasicModule.h"

FilesystemModule::FilesystemModule(INode &node) : NodeModuleDependent(node) {
    setRequired<BasicModule, NodeNetworkModule>();
}

void FilesystemModule::setupActions(ILogicModule::SetupActionHelper &actionHelper) {
    actionHelper.setAction<FileRequestEvent>("fileReq", FileActions::sendFile);
    actionHelper.setAction<FileResponseEvent>("fileRes", FileActions::receivedFile);
    actionHelper.setAction<FileAttributesEvent>("fileAtrS", FileActions::sendAttributes);
    actionHelper.setAction<FileAttributesEvent>("fileAtrR", FileActions::receivedAttributes);
    using namespace std::placeholders;

    actionHelper.setAction<TransferEvent>("beginTransfer",
                                          std::bind(&TransferManager::beginTransfer, &this->transferManager, _1));
    actionHelper.setAction<TransferEvent>("finishTransfer",
                                          std::bind(&TransferManager::finishTransfer, &this->transferManager, _1));
    actionHelper.setAction<TransferEvent>("sendData",
                                          std::bind(&TransferManager::sendData, &this->transferManager, _1));
    actionHelper.setAction<TransferEvent>("sendProps",
                                          std::bind(&TransferManager::transferProperties, &this->transferManager, _1));

}

bool FilesystemModule::assignActions(ILogicModule::AssignActionHelper &actionHelper) {
    if (actionHelper.assignAction<FileRequestEvent>(FileRequestEvent::IdType::GET_FILE, "fileReq") &&
        actionHelper.assignAction<FileRequestEvent>(FileRequestEvent::IdType::GET_CHUNK, "fileReq") &&
        actionHelper.assignAction<FileResponseEvent>(FileResponseEvent::IdType::FILE_RECEIVED, "fileRes") &&
        actionHelper.assignAction<FileResponseEvent>(FileResponseEvent::IdType::CHUNK_RECEIVED, "fileRes") &&
        actionHelper.assignAction<FileAttributesEvent>(FileAttributesEvent::IdType::ATTRIBUTES_REQUESTED, "fileAtrS") &&
        actionHelper.assignAction<FileAttributesEvent>(FileAttributesEvent::IdType::ATTRIBUTES_RECEIVED, "fileAtrR") &&
        actionHelper.assignAction<TransferEvent>(TransferEvent::IdType::REQUESTED, "beginTransfer") &&
        actionHelper.assignAction<TransferEvent>(TransferEvent::IdType::SENDING, "sendData") &&
        actionHelper.assignAction<TransferEvent>(TransferEvent::IdType::GET_PROPERTIES, "sendProps") &&
        actionHelper.assignAction<TransferEvent>(TransferEvent::IdType::FINISHING, "finishTransfer") //&&
            ) {
        std::clog << "Debug: File assignment!" << std::endl;


        //@todo remove this debug:

//        actionHelper.setAction<ConnectionEvent>("fileSendDebug", [](const ConnectionEvent &event) {
//
//            SendFile::Request::Ptr req = SendFile::Request::getNew();
//            req->setFilePath("/tmp/basyco/testfile.txt");
//            req->setBegin(3);
//            req->setEnd(8);
//            event.getConnection()->send(req);
//        });
//
//        if (actionHelper.assignAction<ConnectionEvent>(ConnectionEvent::IdType::CONNECTION_ESTABLISHED,
//                                                       "fileSendDebug")) {
//            std::clog << "Debug: File DEBUG assignment!" << std::endl;
//        }
        return true;
    }
    return false;
}

bool FilesystemModule::setupSources(ILogicModule::SetupSourceHelper &sourceHelper) {
    sourceHelper.requireSource<FileSource>();
    sourceHelper.requireSource<TransferSource>();
    return true;
}

const std::filesystem::path &FilesystemModule::getCurrentPath() const {
    return currentPath;
}

//FileTransferDescriptorPtr FilesystemModule::remoteGetFile(const NodeIdType &nodeId, fs::path from, fs::path to) {
//
//    if (to.is_relative()) {
//        to = currentPath / to;
//    }
//    auto ret = FileTransferControl::initiateTransfer(node, nodeId, from, to);
//
//    currentTransfers.push_back(ret);
//    return ret;
//}
