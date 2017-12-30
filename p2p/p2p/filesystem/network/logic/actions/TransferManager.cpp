//
// Created by stilgar on 30.12.17.
//

#include <p2p/network/protocol/context/NodeContext.h>
#include "TransferManager.h"

//const ResourceIdentificatorPtr &TransferManager::TransferDescriptor::getResourceIdentificatorPtr() const {
//    return resourceIdentificatorPtr;
//}
//
//void TransferManager::TransferDescriptor::setResourceIdentificatorPtr(
//        const ResourceIdentificatorPtr &resourceIdentificatorPtr) {
//    TransferDescriptor::resourceIdentificatorPtr = resourceIdentificatorPtr;
//}

const std::shared_ptr<std::istream> &TransferManager::TransferDescriptor::getInputStream() const {
    return inputStream;
}

void TransferManager::TransferDescriptor::setInputStream(const std::shared_ptr<std::istream> &inputStream) {
    TransferDescriptor::inputStream = inputStream;
}


void TransferManager::beginTransfer(const TransferEvent &event) {
    auto connection = event.origin();
    LOGGER("begin transfer")
    ///@todo find transfer by resource id?

    std::shared_ptr<TransferDescriptor> descriptor = std::make_shared<TransferDescriptor>();

//        descriptor->setResourceIdentificatorPtr(event.getResourceIdentificator());
    if (event.getResourceIdentificator()->exists()) {
        descriptor->setInputStream(event.getResourceIdentificator()->getResourceInputStream());
        ///@todo transfer id generator
        TransferId transferId = transfers.size();
        transfers[transferId] = descriptor;
        NODECONTEXTLOGGER("adding transfer " + std::to_string(transferId));
        BeginTransfer::Response::Ptr response = BeginTransfer::Response::getNew(event.getRequestId());
        response->setTransferId(transferId);

        connection->send(response);


    } else {
        //execute this directly or through source?
        transferError(event);
    }


}
