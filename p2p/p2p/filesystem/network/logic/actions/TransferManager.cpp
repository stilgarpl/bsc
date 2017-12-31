//
// Created by stilgar on 30.12.17.
//

#include <p2p/network/protocol/context/NodeContext.h>
#include <p2p/filesystem/network/packet/DataTransfer.h>
#include <p2p/filesystem/network/packet/PropertiesTransfer.h>
#include "TransferManager.h"

//const ResourceIdentificatorPtr &TransferManager::RemoteTransferDescriptor::getResourceIdentificatorPtr() const {
//    return resourceIdentificatorPtr;
//}
//
//void TransferManager::RemoteTransferDescriptor::setResourceIdentificatorPtr(
//        const ResourceIdentificatorPtr &resourceIdentificatorPtr) {
//    RemoteTransferDescriptor::resourceIdentificatorPtr = resourceIdentificatorPtr;
//}

const std::shared_ptr<std::istream> &TransferManager::RemoteTransferDescriptor::getInputStream() const {
    return inputStream;
}

void TransferManager::RemoteTransferDescriptor::setInputStream(const std::shared_ptr<std::istream> &inputStream) {
    RemoteTransferDescriptor::inputStream = inputStream;
}

TransferSize TransferManager::RemoteTransferDescriptor::getSize() const {
    return size;
}

void TransferManager::RemoteTransferDescriptor::setSize(TransferSize size) {
    RemoteTransferDescriptor::size = size;
}


void TransferManager::beginTransfer(const TransferEvent &event) {
    auto connection = event.origin();
    LOGGER("begin transfer")
    ///@todo find transfer by resource id?

    std::shared_ptr<RemoteTransferDescriptor> descriptor = std::make_shared<RemoteTransferDescriptor>();

//        descriptor->setResourceIdentificatorPtr(event.getResourceIdentificator());
    if (event.getResourceIdentificator()->exists()) {
        descriptor->setInputStream(event.getResourceIdentificator()->getResourceInputStream());

        TransferId transferId = generateTransferId();
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

void TransferManager::finishTransfer(const TransferEvent &event) {
    ///@todo some checking if valid?
    transfers.erase(event.getTransferId());
}

void TransferManager::sendData(const TransferEvent &event) {
    auto connection = event.origin();
    DataTransfer::Response::Ptr response = DataTransfer::Response::getNew(event.getRequestId());
    ///@todo error on bad transfer id
    auto transferDescriptor = transfers[event.getTransferId()];

    std::vector<char> fileContents;
    ///@todo data are copied twice here, isn't there a better way? optimize when we have time
    transferDescriptor->getInputStream()->seekg(event.getBegin());
    char rawData[event.getEnd() - event.getBegin()];
    transferDescriptor->getInputStream()->read(rawData, event.getEnd() - event.getBegin());
    fileContents.assign(rawData, rawData + sizeof rawData / sizeof rawData[0]);

    ///@todo insert ACTUAL begin and end read from stream (in case stream in shorter than requested)
    response->setBegin(event.getBegin());
    response->setEnd(event.getEnd());
    ///@todo make sure this isn't copying the data for the third time
    response->setData(std::move(fileContents));
    connection->send(response);
}

void TransferManager::saveDataChunk(std::shared_ptr<std::ostream> outputStream, const TransferSize &begin,
                                    const TransferSize &end, const RawDataType &data) {


    outputStream->seekp(begin);
    std::copy(data.begin(), data.end(), std::ostreambuf_iterator<char>(*outputStream));


}

void TransferManager::transferProperties(const TransferEvent &event) {
    auto connection = event.origin();
    PropertiesTransfer::Response::Ptr response = PropertiesTransfer::Response::getNew(event.getRequestId());
    auto transfer = transfers[event.getTransferId()];
    if (transfer != nullptr) {
        response->setSize(transfer->getSize());
    }
    connection->send(response);
}

TransferId TransferManager::generateTransferId() {
    static TransferId tid = static_cast<TransferId>(std::rand());
    return ++tid;
}

TransferManager::LocalTransferDescriptorPtr
TransferManager::initiateTransfer(const NodeIdType &nodeId, ResourceIdentificatorPtr source,
                                  ResourceIdentificatorPtr destination) {
    LocalTransferDescriptorPtr ret = std::make_shared<LocalTransferDescriptor>();
    ret->setSource(source);
    ret->setDestination(destination);
    ret->setSourceNode(nodeId);
    ret->startThread([=, &node](LocalTransferDescriptor &descriptorPtr) {
        const size_t MAX_CHUNK_SIZE = 9150;
//        LOGGER("download thread started")
    }
}

