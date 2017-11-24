//
// Created by stilgar on 22.11.17.
//

#ifndef BASYCO_FILETRANSFERCONTROL_H
#define BASYCO_FILETRANSFERCONTROL_H


#include <p2p/log/Logger.h>
#include <p2p/network/node/INode.h>

#include "FileTransferDescriptor.h"



class FileTransferControl {
public:
    [[nodiscard]] static FileTransferDescriptorPtr
    initiateTransfer(INode &node, const NodeIdType &nodeId, fs::path sourcePath, fs::path destinationPath);
};


#endif //BASYCO_FILETRANSFERCONTROL_H
