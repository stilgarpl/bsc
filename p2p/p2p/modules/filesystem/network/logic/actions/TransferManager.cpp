//
// Created by stilgar on 30.12.17.
//

#include <p2p/modules/nodeNetworkModule/protocol/context/NodeContext.h>
#include <p2p/modules/filesystem/network/packet/DataTransfer.h>
#include <p2p/modules/filesystem/network/packet/PropertiesTransfer.h>
#include <p2p/modules/nodeNetworkModule/NodeNetworkModule.h>
#include <p2p/modules/filesystem/network/packet/FinishTransfer.h>
#include <p2p/modules/nodeNetworkModule/remote/RemoteNodeContext.h>
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
    //@todo find transfer by resource id?

    std::shared_ptr<RemoteTransferDescriptor> descriptor = std::make_shared<RemoteTransferDescriptor>();

//        descriptor->setResourceIdentificatorPtr(event.getResourceIdentificator());
    if (event.getResourceIdentificator()->exists()) {
        descriptor->setInputStream(event.getResourceIdentificator()->getResourceInputStream());
        descriptor->setSize(event.getResourceIdentificator()->getResourceSize());

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
    //@todo some checking if valid?
    transfers.erase(event.getTransferId());

    //send response

//    auto remoteNodeContext = event.context()->get<RemoteNodeContext>();
//    if (remoteNodeContext != nullptr) {
//        auto & remoteNode = remoteNodeContext->getRemoteNode();
//        FinishTransfer::Response::Ptr res = FinishTransfer::Response::getNew(event.getRequestId());
//        NODECONTEXTLOGGER("sending finish response")
//        remoteNode.sendPacketToNode(res);
//    } else {
//        NODECONTEXTLOGGER("no remote node context")
//        LOGGER("no node context either?")
//    }

//    auto connectionContext = event.context()->get<ConnectionContext>();
//    if (connectionContext != nullptr) {
//        FinishTransfer::Response::Ptr res = FinishTransfer::Response::getNew(event.getRequestId());
//        connectionContext->getConnection().send(res);
//    } else {
//        LOGGER("no connection context")
//    }

    FinishTransfer::Response::Ptr res = FinishTransfer::Response::getNew(event.getRequestId());
    event.origin()->send(res);
}


void TransferManager::sendData(const TransferEvent &event) {
    auto connection = event.origin();
    DataTransfer::Response::Ptr response = DataTransfer::Response::getNew(event.getRequestId());
    //@todo error on bad transfer id
    auto transferDescriptor = transfers[event.getTransferId()];

    std::vector<char> fileContents;
    //@todo data are copied twice here, isn't there a better way? optimize when we have time
    if (transferDescriptor != nullptr && transferDescriptor->getInputStream() != nullptr) {
        transferDescriptor->getInputStream()->seekg(event.getBegin());
        char rawData[event.getEnd() - event.getBegin()];
        transferDescriptor->getInputStream()->read(rawData, event.getEnd() - event.getBegin());
        fileContents.assign(rawData, rawData + sizeof rawData / sizeof rawData[0]);

        //@todo insert ACTUAL begin and end read from stream (in case stream in shorter than requested)
        response->setBegin(event.getBegin());
        response->setEnd(event.getEnd());
        //@todo make sure this isn't copying the data for the third time
        response->setData(std::move(fileContents));
        connection->send(response);
    }
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
                                  ResourceIdentificatorPtr destination, bool start) {
    INode &node = NodeContext::getNodeFromActiveContext();
    auto networkModule = node.getModule<NodeNetworkModule>();
    LocalTransferDescriptorPtr ret = std::make_shared<LocalTransferDescriptor>();

    Context::Ptr activeContext = Context::getActiveContext();

    ret->setSource(source);
    ret->setDestination(destination);
    ret->setSourceNode(nodeId);
    ret->setPayload([=](LocalTransferDescriptor &descriptorPtr) {
        //before anything, set active context
        Context::setActiveContext(activeContext);


        //@todo get from config
        const TransferSize MAX_CHUNK_SIZE = 19500;
//        LOGGER("download thread started")
        auto destinationStream = destination->getResourceOutputStream();
        //starting transfer
        BeginTransfer::Request::Ptr beginRequest = BeginTransfer::Request::getNew();
        beginRequest->setResourceId(source);
        LOGGER("sending begin req")

        BeginTransfer::Response::Ptr beginResponse = networkModule->sendPacketToNode(nodeId, beginRequest);
        if (beginResponse != nullptr) {
            descriptorPtr.changeState(TransferState::STARTED);
            auto transferId = beginResponse->getTransferId();
//            LOGGER("begin res received")

            PropertiesTransfer::Request::Ptr propertiesRequest = PropertiesTransfer::Request::getNew();
            propertiesRequest->setTransferId(transferId);
//            LOGGER("querying properties")

            PropertiesTransfer::Response::Ptr propertiesResponse = networkModule->sendPacketToNode(nodeId,
                                                                                                   propertiesRequest);
            if (propertiesResponse != nullptr) {
//                LOGGER("got properties")
                descriptorPtr.changeState(TransferState::ATTRIBUTES_ACCQUIRED);
                auto resourceSize = propertiesResponse->getSize();
                SHOW(resourceSize);
                TransferSize chunk_count = resourceSize / MAX_CHUNK_SIZE;
//                LOGGER("chunk count = " + std::to_string(chunk_count));
                descriptorPtr.changeState(TransferState::DOWNLOADING);
                for (TransferSize i = 0; i < chunk_count + 1; ++i) {

                    DataTransfer::Request::Ptr dataRequest = DataTransfer::Request::getNew();
                    dataRequest->setTransferId(transferId);
                    dataRequest->setBegin(i * MAX_CHUNK_SIZE);
//                    LOGGER("begin =" + std::to_string(i * MAX_CHUNK_SIZE));
                    dataRequest->setEnd(std::min((i + 1) * MAX_CHUNK_SIZE, resourceSize));
//                    LOGGER("begin =" + std::to_string(std::min((i + 1) * MAX_CHUNK_SIZE, resourceSize)));
                    DataTransfer::Response::Ptr response = networkModule->sendPacketToNode(nodeId, dataRequest);
                    if (response != nullptr) {
                        // SendFile::Response* response;
                        LOGGER(std::to_string(response->getEnd()) + "/" + std::to_string(resourceSize) + " : " +
                               std::to_string(100 * response->getEnd() / resourceSize) + "%");
                        TransferManager::saveDataChunk(destinationStream, response->getBegin(), response->getEnd(),
                                                       response->getData());
                        descriptorPtr.setTransferredSize(std::min((i + 1) * MAX_CHUNK_SIZE, resourceSize));
//                         std::this_thread::sleep_for(3s);
                    } else {
                        descriptorPtr.changeState(TransferState::ERROR);
                        break;
                    }
                }
            }

            destinationStream->flush();
            FinishTransfer::Request::Ptr finishRequest = FinishTransfer::Request::getNew();
            LOGGER("finishing transfer");
            networkModule->sendPacketToNode(nodeId, finishRequest);
            descriptorPtr.changeState(TransferState::FINISHED);
        }
        LOGGER("transfer finished");
    });
    if (start) {
        ret->startThread();
    }
    localTransfers.push_back(ret);
    return ret;
}


TransferManager::LocalTransferDescriptor::LocalTransferDescriptor()
        : LogicStateMachine(*this) {

    addState(TransferState::NOT_STARTED, TransferState::ATTRIBUTES_ACCQUIRED, TransferState::DOWNLOADING,
             TransferState::STARTED, TransferState::ERROR, TransferState::FINISHED);
    addLink(TransferState::NOT_STARTED, TransferState::STARTED, TransferState::ERROR);
    addLink(TransferState::STARTED, TransferState::ATTRIBUTES_ACCQUIRED, TransferState::ERROR);
    addLink(TransferState::ATTRIBUTES_ACCQUIRED, TransferState::DOWNLOADING, TransferState::ERROR);
    addLink(TransferState::DOWNLOADING, TransferState::FINISHED, TransferState::ERROR);
    addLink(TransferState::ERROR, TransferState::NOT_STARTED);
    setState(TransferState::NOT_STARTED);


}

void TransferManager::TransferQueue::queueTransfer(const NodeIdType &nodeId, ResourceIdentificatorPtr source,
                                                   ResourceIdentificatorPtr destination) {
    transfers.push_back(manager.initiateTransfer(nodeId, source, destination, false));
}
