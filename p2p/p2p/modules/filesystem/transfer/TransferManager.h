#include <utility>

//
// Created by stilgar on 30.12.17.
//

#ifndef BASYCO_TRANSFERMANAGER_H
#define BASYCO_TRANSFERMANAGER_H


#include <p2p/modules/filesystem/network/logic/events/TransferEvent.h>
#include <logic/state/LogicStateMachine.h>
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
        PREPARED,
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
        std::mutex threadStartMutex;
        std::chrono::time_point<std::chrono::steady_clock> startTime, endTime;

        //very private, no getters:

        std::unique_ptr<std::thread> thread = nullptr;

        std::function<void(LocalTransferDescriptor &)> payload;

    protected:

        void setPayload(const std::function<void(LocalTransferDescriptor &)> &p);

        void startThread();

        friend class TransferManager;

    public:
        const ResourceIdentificatorPtr &getDestination() const;

        void setDestination(const ResourceIdentificatorPtr &destination);

        const ResourceIdentificatorPtr &getSource() const;

        void setSource(const ResourceIdentificatorPtr &source);

        const NodeIdType &getSourceNode() const;

        void setSourceNode(const NodeIdType &sourceNode);

        bool isStarted();

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

        const std::chrono::time_point<std::chrono::steady_clock> &getStartTime() const;

        void setStartTime(const std::chrono::time_point<std::chrono::steady_clock> &startTime);

        const std::chrono::time_point<std::chrono::steady_clock> &getEndTime() const;

        void setEndTime(const std::chrono::time_point<std::chrono::steady_clock> &endTime);

        ~LocalTransferDescriptor() override;

        void wait();

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
        const unsigned long MAX_CONCURRENT_TRANSFERS = 2;
        std::mutex finishLock;
        std::condition_variable finishReady;
        std::recursive_mutex startLock;

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
        queueTransfer(const NodeIdType &nodeId, const ResourceIdentificatorPtr &source,
                      const ResourceIdentificatorPtr &destination);

        void queueTransfer(ResourceIdentificatorPtr source, ResourceIdentificatorPtr destination);
        void start();

        void waitToFinishAllTransfers();
    };

    using TransferQueuePtr = std::shared_ptr<TransferQueue>;


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
    saveDataChunk(const std::shared_ptr<std::ostream> &outputStream, const TransferSize &begin, const TransferSize &end,
                  const RawDataType &data);

    std::shared_ptr<TransferQueue> transferQueue() {
        //@todo store this transfer queue somewhere?
        return std::make_shared<TransferQueue>(*this);
    }

    [[nodiscard]]  LocalTransferDescriptorPtr
    initiateTransfer(const NodeIdType &nodeId, const ResourceIdentificatorPtr &source,
                     const ResourceIdentificatorPtr &destination,
                     bool start = true);

    [[nodiscard]]  LocalTransferDescriptorPtr
    initiateTransfer(ResourceIdentificatorPtr source, ResourceIdentificatorPtr destination,
                     bool start = true);

};


#endif //BASYCO_TRANSFERMANAGER_H
