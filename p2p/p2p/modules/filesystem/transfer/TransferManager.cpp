#include <utility>

//
// Created by stilgar on 30.12.17.
//

#include <p2p/node/context/NodeContext.h>
#include <p2p/modules/filesystem/network/packet/DataTransfer.h>
#include <p2p/modules/filesystem/network/packet/PropertiesTransfer.h>
#include <p2p/modules/network/NetworkModule.h>
#include <p2p/modules/filesystem/network/packet/FinishTransfer.h>
#include <p2p/modules/filesystem/network/packet/TransferQuery.h>
#include <p2p/modules/filesystem/FilesystemModule.h>
#include "TransferManager.h"
#include "TransferException.h"
#include <p2p/modules/filesystem/network/packet/BeginTransfer.h>
#include <p2p/modules/network/protocol/context/ConnectionContext.h>

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
    auto& connectionContext = Context::getActiveContext()->get<ConnectionContext>();
    Connection& connection = connectionContext.getConnection();
    LOGGER("begin transfer")
    //@todo find transfer by resource id?

    std::shared_ptr<RemoteTransferDescriptor> descriptor = std::make_shared<RemoteTransferDescriptor>();

//        descriptor->setResourceIdentificatorPtr(event.getResourceIdentificator());
    if (event.getResourceIdentificator()->exists()) {
        descriptor->setInputStream(event.getResourceIdentificator()->getResourceInputStream());
        descriptor->setSize(event.getResourceIdentificator()->getResourceSize());

        TransferId transferId = generateTransferId();
        transfers[transferId] = descriptor;
        LOGGER ("adding transfer " + std::to_string(transferId));
        BeginTransfer::Response::Ptr response = BeginTransfer::Response::getNew(event.getRequestId());
        response->setTransferId(transferId);

        connection.send(response);


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
//        LOGGER("sending finish response")
//        remoteNode.sendPacketToNode(res);
//    } else {
//        LOGGER("no remote node context")
//        LOGGER("no node context either?")
//    }

//    auto connectionContext = event.context()->get<ConnectionContext>();
//    if (connectionContext != nullptr) {
//        FinishTransfer::Response::Ptr res = FinishTransfer::Response::getNew(event.getRequestId());
//        connectionContext.getConnection().send(res);
//    } else {
//        LOGGER("no connection context")
//    }

    FinishTransfer::Response::Ptr res = FinishTransfer::Response::getNew(event.getRequestId());
    auto& connectionContext = Context::getActiveContext()->get<ConnectionContext>();
    Connection& connection = connectionContext.getConnection();
    connection.send(res);
}


void TransferManager::sendData(const TransferEvent &event) {
    auto& connectionContext = Context::getActiveContext()->get<ConnectionContext>();
    Connection& connection = connectionContext.getConnection();
    DataTransfer::Response::Ptr response = DataTransfer::Response::getNew(event.getRequestId());
    //@todo error on bad transfer id
    auto transferDescriptor = transfers[event.getTransferId()];

    std::vector<char> &fileContents = response->getData();
    if (transferDescriptor != nullptr && transferDescriptor->getInputStream() != nullptr) {
        transferDescriptor->getInputStream()->seekg(event.getBegin());
        fileContents.resize(event.getEnd() - event.getBegin());
        transferDescriptor->getInputStream()->read(fileContents.data(), event.getEnd() - event.getBegin());
        //@todo insert ACTUAL begin and end read from stream (in case stream is shorter than requested)
        response->setBegin(event.getBegin());
        response->setEnd(event.getEnd());
        connection.send(response);
    }
}

void TransferManager::saveDataChunk(const std::shared_ptr<std::ostream> &outputStream, const TransferSize &begin,
                                    const TransferSize &end, const RawDataType &data) {


    outputStream->seekp(begin);
    std::copy(data.begin(), data.end(), std::ostreambuf_iterator<char>(*outputStream));


}

void TransferManager::transferProperties(const TransferEvent &event) {
    auto& connectionContext = Context::getActiveContext()->get<ConnectionContext>();
    Connection& connection = connectionContext.getConnection();
    PropertiesTransfer::Response::Ptr response = PropertiesTransfer::Response::getNew(event.getRequestId());
    auto transfer = transfers[event.getTransferId()];
    if (transfer != nullptr) {
        response->setSize(transfer->getSize());
    }
    connection.send(response);
}

TransferId TransferManager::generateTransferId() {
    static auto tid = static_cast<TransferId>(std::rand());
    return ++tid;
}

TransferManager::LocalTransferDescriptorPtr
TransferManager::initiateTransfer(const NodeIdType &nodeId, const ResourceIdentificatorPtr &source,
                                  const ResourceIdentificatorPtr &destination, bool start) {
    INode &node = NodeContext::getNodeFromActiveContext();
    auto networkModule = node.getModule<NetworkModule>();
    LocalTransferDescriptorPtr ret = std::make_shared<LocalTransferDescriptor>();

    Context::Ptr activeContext = Context::getActiveContext();

    ret->setSource(source);
    ret->setDestination(destination);
    ret->setSourceNode(nodeId);
    ret->setPayload([=](LocalTransferDescriptor &descriptorPtr) {
        //before anything, setDirect active context
        Context::setActiveContext(activeContext);
        LOGGER("starting transfer...")

        const TransferSize MAX_CHUNK_SIZE = NodeContext::getNodeFromActiveContext().getModule<FilesystemModule>()->configuration().maxChunkSize;
//        LOGGER("download thread started")
        auto destinationStream = destination->getResourceOutputStream();
        //starting transfer
        BeginTransfer::Request::Ptr beginRequest = BeginTransfer::Request::getNew();
        beginRequest->setResourceId(source);
        LOGGER("sending begin req")

        BeginTransfer::Response::Ptr beginResponse = networkModule->sendPacketToNode(nodeId, beginRequest);
        if (beginResponse != nullptr) {
            LOGGER("transfer manager -> STARTED")
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
                LOGGER("transfer manager -> AA")
                descriptorPtr.changeState(TransferState::ATTRIBUTES_ACCQUIRED);
                auto resourceSize = propertiesResponse->getSize();
                SHOW(resourceSize);
                TransferSize chunk_count = resourceSize / MAX_CHUNK_SIZE;
//                LOGGER("chunk count = " + std::to_string(chunk_count));
                LOGGER("transfer manager -> DOWNLOADING")
                descriptorPtr.changeState(TransferState::DOWNLOADING);
                for (TransferSize i = 0; i < chunk_count + 1; ++i) {

                    DataTransfer::Request::Ptr dataRequest = DataTransfer::Request::getNew();
                    dataRequest->setTransferId(transferId);
                    dataRequest->setBegin(i * MAX_CHUNK_SIZE);
//                    LOGGER("begin =" + std::to_string(i * MAX_CHUNK_SIZE));
                    dataRequest->setEnd(std::min((i + 1) * MAX_CHUNK_SIZE, resourceSize));
//                    LOGGER("begin =" + std::to_string(std::min((i + 1) * MAX_CHUNK_SIZE, resourceSize)));
                    LOGGER("requesting data chunk ")
                    auto response = networkModule->sendPacketToNode(nodeId, dataRequest);
                    if (response != nullptr) {
                        // SendFile::Response* response;
                        LOGGER(std::to_string(response->getEnd()) + "/" + std::to_string(resourceSize) + " : " +
                               std::to_string(100 * response->getEnd() / resourceSize) + "%");
                        TransferManager::saveDataChunk(destinationStream, response->getBegin(), response->getEnd(),
                                                       response->getData());
                        descriptorPtr.setTransferredSize(std::min((i + 1) * MAX_CHUNK_SIZE, resourceSize));
//                         std::this_thread::sleep_for(3s);
                    } else {
                        LOGGER("no response for request, transfer manager -> error")
                        descriptorPtr.changeState(TransferState::ERROR);
                        break;
                    }
                }
            }

            destinationStream->flush();
            FinishTransfer::Request::Ptr finishRequest = FinishTransfer::Request::getNew();
            LOGGER("finishing transfer");
            networkModule->sendPacketToNode(nodeId, finishRequest);
            LOGGER("transfer manager -> FINISHED")
            descriptorPtr.changeState(TransferState::FINISHED);
        }
        LOGGER("transfer finished ");
    });
    if (start) {
        ret->startThread();
    }
    localTransfers.push_back(ret);
    return ret;
}

TransferManager::LocalTransferDescriptorPtr
TransferManager::initiateTransfer(ResourceIdentificatorPtr source, ResourceIdentificatorPtr destination, bool start) {
    auto netModule = NodeContext::getNodeFromActiveContext().getModule<NetworkModule>();
    //@todo this broadcast shouldn't happen at the moment of ordering the request, but at the time of actual run. but it's not high priority, most of the time it won't matter
    //broadcast who has
    TransferQuery::Request::Ptr request = TransferQuery::Request::getNew();
    request->setResourceIdentificator(source);
    //@todo this will block... maybe do it in async way? it would be best to do all the broadcast stuff at the start of the transfer thread
    auto responseMap = netModule->broadcastRequest(request);
    //select best node.
    //@todo actual strategy to select the best node - for example, a object that takes this map and returns best nodeId
    for (auto &&[nodeId, response] : responseMap) {
//        LOGGER("response from " + nodeId)
        if (response->isExists()) {
            //initiate transfer
            return initiateTransfer(nodeId, source, destination, start);
        }
    }
    LOGGER("no node has the resource?")
    return nullptr;

}


TransferManager::LocalTransferDescriptor::LocalTransferDescriptor()
        : LogicStateMachine(*this) {

    addState(TransferState::NOT_STARTED, TransferState::PREPARED, TransferState::ATTRIBUTES_ACCQUIRED,
             TransferState::DOWNLOADING,
             TransferState::STARTED, TransferState::ERROR, TransferState::FINISHED);
    addLink(TransferState::NOT_STARTED, TransferState::PREPARED, TransferState::STARTED, TransferState::ERROR);
    addLink(TransferState::PREPARED, TransferState::STARTED, TransferState::ERROR);
    addLink(TransferState::STARTED, TransferState::ATTRIBUTES_ACCQUIRED, TransferState::ERROR);
    addLink(TransferState::ATTRIBUTES_ACCQUIRED, TransferState::DOWNLOADING, TransferState::ERROR);
    addLink(TransferState::DOWNLOADING, TransferState::FINISHED, TransferState::ERROR);
    addLink(TransferState::ERROR, TransferState::NOT_STARTED);
    setState(TransferState::NOT_STARTED);


}

void TransferManager::LocalTransferDescriptor::setPayload(const std::function<void(LocalTransferDescriptor &)> &p) {
    payload = p;
}

void TransferManager::LocalTransferDescriptor::startThread() {
    std::lock_guard<std::mutex> g(threadStartMutex);
    if (thread == nullptr) {
        changeState(TransferState::PREPARED);
        thread = std::make_unique<std::thread>([=, this](LocalTransferDescriptor &descriptor) {
            descriptor.setStartTime(std::chrono::steady_clock::now());
            payload(descriptor);
            descriptor.setEndTime(std::chrono::steady_clock::now());

            //@todo debug, move somewhere else
            std::chrono::milliseconds time = std::chrono::duration_cast<std::chrono::milliseconds>(
                    descriptor.getEndTime() - descriptor.getStartTime());
            auto dataTransferred = descriptor.getTransferredSize();
            auto speed = dataTransferred / time.count();
            LOGGER(std::string("Transferred bytes: ") + std::to_string(dataTransferred) + " in " +
                   std::to_string(time.count()) + "ms. Speed " + std::to_string(speed) + "KB/s")
        }, std::ref(*this));
    } else {
        //@todo error already started
        LOGGER("thread already started!!!")
    }
    //f(*this);
}

const ResourceIdentificatorPtr &TransferManager::LocalTransferDescriptor::getDestination() const {
    return destination;
}

void TransferManager::LocalTransferDescriptor::setDestination(const ResourceIdentificatorPtr &destination) {
    LocalTransferDescriptor::destination = destination;
}

const ResourceIdentificatorPtr &TransferManager::LocalTransferDescriptor::getSource() const {
    return source;
}

void TransferManager::LocalTransferDescriptor::setSource(const ResourceIdentificatorPtr &source) {
    LocalTransferDescriptor::source = source;
}

const NodeIdType &TransferManager::LocalTransferDescriptor::getSourceNode() const {
    return sourceNode;
}

void TransferManager::LocalTransferDescriptor::setSourceNode(const NodeIdType &sourceNode) {
    LocalTransferDescriptor::sourceNode = sourceNode;
}

TransferManager::LocalTransferDescriptor::~LocalTransferDescriptor() {
    std::lock_guard<std::mutex> g(threadStartMutex);
    //@todo kill the thread or something.
    if (thread != nullptr && thread->joinable()) {
        thread->join();
    }
}

void TransferManager::LocalTransferDescriptor::wait() {
    std::lock_guard<std::mutex> g(threadStartMutex);
    if (thread != nullptr && thread->joinable()) {
        thread->join();
    }
}

bool TransferManager::LocalTransferDescriptor::isStarted() {
    std::lock_guard<std::mutex> g(threadStartMutex);
    return thread != nullptr;
}

const std::chrono::time_point<std::chrono::steady_clock> &
TransferManager::LocalTransferDescriptor::getStartTime() const {
    return startTime;
}

void TransferManager::LocalTransferDescriptor::setStartTime(
        const std::chrono::time_point<std::chrono::steady_clock> &startTime) {
    LocalTransferDescriptor::startTime = startTime;
}

const std::chrono::time_point<std::chrono::steady_clock> &TransferManager::LocalTransferDescriptor::getEndTime() const {
    return endTime;
}

void TransferManager::LocalTransferDescriptor::setEndTime(
        const std::chrono::time_point<std::chrono::steady_clock> &endTime) {
    LocalTransferDescriptor::endTime = endTime;
}

void TransferManager::TransferQueue::queueTransfer(const NodeIdType &nodeId, const ResourceIdentificatorPtr &source,
                                                   const ResourceIdentificatorPtr &destination) {
    auto transfer = manager.initiateTransfer(nodeId, source, destination, false);
    transfer->registerStateObserver(*this);
    transfers.push_back(transfer);
}

void TransferManager::TransferQueue::update(TransferManager::LocalTransferDescriptor &object,
                                            TransferManager::TransferState state) {
    LOGGER("transfer queue update start")
    switch (state) {

        case TransferState::NOT_STARTED:
        case TransferState::PREPARED:
            //does not apply
            LOGGER("..")
            break;
        case TransferState::STARTED:
            //if at least one transfer is started
            LOGGER("transfer queue -> STARTED")
            changeState(TransferState::STARTED);
            break;
        case TransferState::ATTRIBUTES_ACCQUIRED:
            //this does not apply to transfer queue
            break;
        case TransferState::DOWNLOADING:
            //if at least one transfer is in downloading state -- and since we got update, one definitely is
            LOGGER("transfer queue -> DOWNLOADING")
            changeState(TransferState::DOWNLOADING);
            break;
        case TransferState::FINISHED:
            //when all transfers from queue are finished, the queue is finished.
            if (countUnfinishedTransfers() == 0) {
                LOGGER("transfer queue -> FINISHED")
                changeState(TransferState::FINISHED);
                LOGGER("transfer queue finished")
                finishReady.notify_all();
            } else {
                //start more transfers
                start();
            }
            //transfer was finished, ignoring further events from it. @todo maybe use RAII for this?
            object.unregisterStateObserver(*this);
            break;
        case TransferState::ERROR:
            //@todo handle error
            changeState(TransferState::ERROR);
            finishReady.notify_all();
            //@todo R U sure?
            object.unregisterStateObserver(*this);
            break;
    }

    LOGGER("transfer queue update done!")
    if (state == TransferState::FINISHED) {

    }

}

TransferManager::TransferQueue::TransferQueue(TransferManager &manager) : LogicStateMachine(*this), manager(manager) {

    addState(TransferState::NOT_STARTED, TransferState::PREPARED, TransferState::ATTRIBUTES_ACCQUIRED,
             TransferState::DOWNLOADING,
             TransferState::STARTED, TransferState::ERROR, TransferState::FINISHED);
    addLink(TransferState::NOT_STARTED, TransferState::PREPARED, TransferState::STARTED, TransferState::ERROR);
    addLink(TransferState::PREPARED, TransferState::STARTED, TransferState::ERROR);
    addLink(TransferState::STARTED, TransferState::ATTRIBUTES_ACCQUIRED, TransferState::DOWNLOADING,
            TransferState::ERROR);
    addLink(TransferState::ATTRIBUTES_ACCQUIRED, TransferState::DOWNLOADING, TransferState::ERROR);
    addLink(TransferState::DOWNLOADING, TransferState::FINISHED, TransferState::ERROR);
    addLink(TransferState::FINISHED, TransferState::NOT_STARTED, TransferState::ERROR);
    addLink(TransferState::ERROR, TransferState::NOT_STARTED);
    setState(TransferState::NOT_STARTED);

}

void TransferManager::TransferQueue::start() {
//    LOGGER("transfers in not started :" + std::to_string(countTransfersNotInState(TransferState::NOT_STARTED)));
//    LOGGER("transfers in finished :" + std::to_string(countTransfersInState(TransferState::FINISHED)));
//    LOGGER("all transfers " + std::to_string(transfers.size()));
//    unsigned long  transfersToStart = std::min(countTransfersNotInState(TransferState::NOT_STARTED) - countTransfersInState(TransferState::FINISHED),MAX_CONCURRENT_TRANSFERS);
    std::lock_guard<std::recursive_mutex> g(startLock);
    auto transfersNotStarted = countTransfersInState(TransferState::NOT_STARTED);
    auto transfersFinished = countTransfersInState(TransferState::FINISHED);
    auto transfersRunning = transfers.size() - transfersFinished - transfersNotStarted;

    auto transfersToStart = std::min<decltype(transfers.size())>(
            std::max<decltype(transfers.size())>(MAX_CONCURRENT_TRANSFERS - transfersRunning, 0),
            transfersNotStarted);
//    LOGGER("transfers to start " + std::to_string(transfersToStart));
    LOGGER("Transfer queue start, transfers to start=" + std::to_string(transfersToStart))
    //@todo actual transfer policy
    for (const auto &item : transfers) {
        if (!item->isStarted()) {
            LOGGER("transfer not started, starting ")
            item->startThread();
            transfersToStart--;
        }
        if (transfersToStart == 0) {
            break;
        }
    }
    if (transfersToStart > 0) {
        LOGGER("some transfers were not started " + std::to_string(transfersToStart))
        throw TransferException("Some transfers were not started :" + std::to_string(transfersToStart));
    }
}

auto TransferManager::TransferQueue::countTransfersNotInState(TransferManager::TransferState state) -> decltype(transfers.size()){
    auto result = transfers.size();
    for (const auto &item : transfers) {
        if (item->getCurrentState() == state) {
            result--;
        }
    }
    return result;
}

auto TransferManager::TransferQueue::countTransfersInState(TransferManager::TransferState state) -> decltype(transfers.size()){
    decltype(transfers.size()) result = 0;
    for (const auto &item : transfers) {
        if (item->getCurrentState() == state) {
            result++;
        }
    }
    return result;
}

auto TransferManager::TransferQueue::countUnfinishedTransfers() -> decltype(transfers.size()) {
//    unsigned long result = transfers.size();
//    for (const auto &item : transfers) {
//        if (item->getCurrentState() == TransferState::FINISHED) {
//            result--;
//        }
//    }
//    return result;
    return countTransfersNotInState(TransferState::FINISHED);
}

void
TransferManager::TransferQueue::queueTransfer(ResourceIdentificatorPtr source, ResourceIdentificatorPtr destination) {
    {
        //@todo I'm not sure if this should be here or somewhere else
        std::unique_lock<std::mutex> g(finishLock);
        if (getCurrentState() == TransferState::FINISHED) {
            LOGGER("transfer queue was finished, resetting")
            changeState(TransferState::NOT_STARTED);
            LOGGER("not started")
        }
    }
    auto transfer = manager.initiateTransfer(std::move(source), std::move(destination), false);
    if (transfer != nullptr) {
        transfer->registerStateObserver(*this);
        transfers.push_back(transfer);
    } else {
        LOGGER("failed to initiate transfer!")
        throw TransferException("Failed to initiate transfer");
    }
}

void TransferManager::TransferQueue::waitToFinishAllTransfers() {
    std::unique_lock<std::mutex> g(finishLock);

    finishReady.wait(g, [this] {
        return this->getCurrentState() == TransferState::FINISHED || this->getCurrentState() == TransferState::ERROR;
    });

    if (this->getCurrentState() == TransferState::ERROR) {
        LOGGER("current state is error, terminating")
        throw TransferException("Transfer error");
    }

}
