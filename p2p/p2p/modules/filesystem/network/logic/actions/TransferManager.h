#include <utility>

//
// Created by stilgar on 30.12.17.
//

#ifndef BASYCO_TRANSFERMANAGER_H
#define BASYCO_TRANSFERMANAGER_H


#include <p2p/modules/filesystem/network/logic/events/TransferEvent.h>
#include <p2p/modules/filesystem/network/packet/BeginTransfer.h>
#include <p2p/logic/state/LogicStateMachine.h>
#include <algorithm>

class TransferManager {
    /**
     * transfer descriptor on server side. created when beginTransfer is used, deleted when finishTransfer is used
     */
    class RemoteTransferDescriptor {
        std::shared_ptr<std::istream> inputStream;
        TransferSize size;

    public:
        const std::shared_ptr<std::istream> &getInputStream() const;

        void setInputStream(const std::shared_ptr<std::istream> &inputStream);

        TransferSize getSize() const;

        void setSize(TransferSize size);
    };

public:

    enum class TransferState {
        NOT_STARTED,
        STARTED,
        ATTRIBUTES_ACCQUIRED,
        DOWNLOADING,
        FINISHED,
        ERROR,
    };

    /**
     * transfer descriptor on local side, with its own thread that handles the downloading of the entire source stream from the other node
     */
    class LocalTransferDescriptor : protected LogicStateMachine<LocalTransferDescriptor, TransferState> {

        ResourceIdentificatorPtr destination;
        ResourceIdentificatorPtr source;
        NodeIdType sourceNode;
        TransferSize size;
        TransferSize transferredSize;

        //very private, no getters:

        std::unique_ptr<std::thread> thread = nullptr;

        std::function<void(LocalTransferDescriptor &)> payload;

    protected:

        void setPayload(const std::function<void(LocalTransferDescriptor &)> &p) {
            payload = p;
        }

        void startThread() {
            if (thread == nullptr) {
                thread = std::make_unique<std::thread>(payload, std::ref(*this));
            } else {
                //@todo error already started
                LOGGER("thread already started!!!")
            }
            //f(*this);
        }

        friend class TransferManager;

    public:
        const ResourceIdentificatorPtr &getDestination() const {
            return destination;
        }

        void setDestination(const ResourceIdentificatorPtr &destination) {
            LocalTransferDescriptor::destination = destination;
        }

        const ResourceIdentificatorPtr &getSource() const {
            return source;
        }

        void setSource(const ResourceIdentificatorPtr &source) {
            LocalTransferDescriptor::source = source;
        }

        const NodeIdType &getSourceNode() const {
            return sourceNode;
        }

        void setSourceNode(const NodeIdType &sourceNode) {
            LocalTransferDescriptor::sourceNode = sourceNode;
        }

        TransferSize getSize() const {
            return size;
        }

        void setSize(TransferSize size) {
            LocalTransferDescriptor::size = size;
        }

        TransferSize getTransferredSize() const {
            return transferredSize;
        }

        void setTransferredSize(TransferSize transferredSize) {
            LocalTransferDescriptor::transferredSize = transferredSize;
        }

        virtual ~LocalTransferDescriptor() {
            //@todo kill the thread or something.
            if (thread != nullptr && thread->joinable()) {
                thread->join();
            }
        }

        void wait() {
            if (thread != nullptr && thread->joinable()) {
                thread->join();
            }
        }

        friend class TransferQueue;

        LocalTransferDescriptor();
    };


public:
    typedef std::shared_ptr<LocalTransferDescriptor> LocalTransferDescriptorPtr;

    /**
     * a collection of transfers, downloading them sequentially or more at a time
     */
    class TransferQueue
            : public LocalTransferDescriptor::Observer, public LogicStateMachine<TransferQueue, TransferState> {
    private:
        std::list<LocalTransferDescriptorPtr> transfers;
        TransferManager &manager;
        unsigned long MAX_CONCURRENT_TRANSFERS = 2;

    public:
        explicit TransferQueue(TransferManager &manager);

        void update(LocalTransferDescriptor &object, TransferState type) override;

    protected:

        //@todo this method probably can be made more efficient or omitted entirely
        unsigned long countUnfinishedTransfers();

        unsigned long countTransfersInState(TransferState state);

        unsigned long countTransfersNotInState(TransferState state);

    public:
        void
        queueTransfer(const NodeIdType &nodeId, ResourceIdentificatorPtr source, ResourceIdentificatorPtr destination);

        void queueTransfer(ResourceIdentificatorPtr source, ResourceIdentificatorPtr destination);
        void start();
    };


private:
    TransferId generateTransferId();

    std::map<TransferId, std::shared_ptr<RemoteTransferDescriptor>> transfers;
    std::list<LocalTransferDescriptorPtr> localTransfers;

public:
    void beginTransfer(const TransferEvent &event);

    void transferError(const TransferEvent &event) {
//@todo
    }

    void finishTransfer(const TransferEvent &event);

    void sendData(const TransferEvent &event);

    void transferProperties(const TransferEvent &event);

    static void
    saveDataChunk(std::shared_ptr<std::ostream> outputStream, const TransferSize &begin, const TransferSize &end,
                  const RawDataType &data);

    std::shared_ptr<TransferQueue> transferQueue() {
        //@todo store this transfer queue somewhere?
        return std::make_shared<TransferQueue>(*this);
    }

    [[nodiscard]]  LocalTransferDescriptorPtr
    initiateTransfer(const NodeIdType &nodeId, ResourceIdentificatorPtr source, ResourceIdentificatorPtr destination,
                     bool start = true);

    [[nodiscard]]  LocalTransferDescriptorPtr
    initiateTransfer(ResourceIdentificatorPtr source, ResourceIdentificatorPtr destination,
                     bool start = true);

};


#endif //BASYCO_TRANSFERMANAGER_H
