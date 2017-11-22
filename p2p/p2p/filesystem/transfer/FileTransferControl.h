//
// Created by stilgar on 22.11.17.
//

#ifndef BASYCO_FILETRANSFERCONTROL_H
#define BASYCO_FILETRANSFERCONTROL_H


#include <p2p/log/Logger.h>
#include <p2p/network/node/Node.h>
#include <p2p/network/node/modules/NodeNetworkModule.h>
#include <p2p/filesystem/network/packet/SendAttributes.h>
#include <p2p/network/protocol/packet/info/PacketInfo.h>
#include <p2p/network/protocol/packet/BasePacket.h>
#include "FileTransferDescriptor.h"
#include <p2p/filesystem/network/packet/SendFile.h>
#include <p2p/network/node/modules/FilesystemModule.h>


class FileTransferControl {
public:
    [[nodiscard]] static FileTransferDescriptorPtr
    initiateTransfer(Node &node, const NodeIdType &nodeId, fs::path sourcePath, fs::path destinationPath) {
        FileTransferDescriptorPtr ret = std::make_shared<FileTransferDescriptor>();
        ret->setSourceNode(nodeId);
        ret->setSourcePath(sourcePath);
        ret->setDestinationPath(destinationPath);
        ret->startThread([=, &node](FileTransferDescriptor &descriptorPtr) {
            const size_t MAX_CHUNK_SIZE = 9150;
            LOGGER("download thread started")
            ret->setStatus(FileTransferStatus::STARTED);
            SendAttributes::Request::Ptr req = SendAttributes::Request::getNew();
            //SendAttributes::Request * req;
            req->setPath(sourcePath);
            auto res = node.getModule<NodeNetworkModule>()->sendPacketToNode(nodeId, req);

            if (res != nullptr) {
                //SendAttributes::Response * res;
                LOGGER("size = " + std::to_string((int) res->getSize()));
                size_t file_size = res->getSize();
                SendFile::Request::Ptr sendFileRequest = SendFile::Request::getNew();
                //   SendFile::Request* sendFileRequest;
                sendFileRequest->setFilePath(sourcePath);
                auto to = destinationPath;
                if (to.is_relative()) {
                    to = node.getModule<FilesystemModule>()->getCurrentPath() / to;
                }
                auto netnode = node.getModule<NodeNetworkModule>();
                if (file_size <= MAX_CHUNK_SIZE) {


                    auto response = netnode->sendPacketToNode(nodeId, sendFileRequest);

                    // SendFile::Response* response;
                    response->setFilePath(to);
                    response->save_file();
                } else {
                    size_t chunk_count = file_size / MAX_CHUNK_SIZE;
                    LOGGER("chunk count = " + std::to_string(chunk_count));
                    for (int i = 0; i < chunk_count + 1; ++i) {
                        sendFileRequest->setBegin(i * MAX_CHUNK_SIZE);
                        LOGGER("begin =" + std::to_string(i * MAX_CHUNK_SIZE));
                        sendFileRequest->setEnd(std::min((i + 1) * MAX_CHUNK_SIZE, file_size));
                        LOGGER("begin =" + std::to_string(std::min((i + 1) * MAX_CHUNK_SIZE, file_size)));
                        auto response = netnode->sendPacketToNode(nodeId, sendFileRequest);

                        // SendFile::Response* response;
                        response->setFilePath(to);
                        response->save_file();
                        // std::this_thread::sleep_for(3s);
                    }

                }
            }
        });

        return ret;
    }
};


#endif //BASYCO_FILETRANSFERCONTROL_H
