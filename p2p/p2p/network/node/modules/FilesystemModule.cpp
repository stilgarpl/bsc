//
// Created by stilgar on 07.11.17.
//

#include <p2p/filesystem/network/logic/sources/FileSource.h>
#include <p2p/filesystem/network/logic/actions/FileActions.h>
#include <p2p/network/protocol/logic/events/ConnectionEvent.h>
#include "FilesystemModule.h"

FilesystemModule::FilesystemModule(INode &node) : NodeModule(node) {}

void FilesystemModule::setupActions(LogicManager &logicManager) {
    logicManager.setAction<FileRequestEvent>("fileReq", FileActions::sendFile);
    logicManager.setAction<FileResponseEvent>("fileRes", FileActions::receivedFile);

}

bool FilesystemModule::assignActions(LogicManager &logicManager) {
    if (logicManager.assignAction<FileRequestEvent>(FileRequestEvent::IdType::GET_FILE, "fileReq") &&
        logicManager.assignAction<FileRequestEvent>(FileRequestEvent::IdType::GET_CHUNK, "fileReq") &&
        logicManager.assignAction<FileResponseEvent>(FileResponseEvent::IdType::FILE_RECEIVED, "fileRes") &&
        logicManager.assignAction<FileResponseEvent>(FileResponseEvent::IdType::CHUNK_RECEIVED, "fileRes")) {
        std::clog << "Debug: File assignment!" << std::endl;


        ///@todo remove this debug:

        logicManager.setAction<ConnectionEvent>("fileSendDebug", [](const ConnectionEvent &event) {

            SendFile::Request::Ptr req = SendFile::Request::getNew();
            req->setFilePath("/tmp/basyco/testfile.txt");
            req->setBegin(3);
            req->setEnd(8);
            event.getConnection()->send(req);
        });

        if (logicManager.assignAction<ConnectionEvent>(ConnectionEvent::IdType::CONNECTION_ESTABLISHED,
                                                       "fileSendDebug")) {
            std::clog << "Debug: File DEBUG assignment!" << std::endl;
        }
        return true;
    }
    return false;
}

bool FilesystemModule::setupSources(LogicManager &logicManager) {
    logicManager.addSource<FileSource>();
    return true;
}
