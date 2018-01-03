//
// Created by stilgar on 30.12.17.
//

#ifndef BASYCO_TRANSFERMANAGER_H
#define BASYCO_TRANSFERMANAGER_H


#include <p2p/filesystem/network/logic/events/TransferEvent.h>
#include <p2p/filesystem/network/packet/BeginTransfer.h>

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
    /**
     * transfer descriptor on local side, with its own thread that handles the downloading of the entire source stream from the other node
     */
    class LocalTransferDescriptor {

        enum class TransferStatus {
            NOT_STARTED,
            STARTED,
            ATTRIBUTES_ACCQUIRED,
            DOWNLOADING,
            FINISHED,
            ERROR,
        };

        TransferStatus status = TransferStatus::NOT_STARTED;
        ResourceIdentificatorPtr destination;
        ResourceIdentificatorPtr source;
        NodeIdType sourceNode;
        TransferSize size;
        TransferSize transferredSize;

        //very private, no getters:

        std::unique_ptr<std::thread> thread = nullptr;

    protected:

        void startThread(std::function<void(LocalTransferDescriptor &)> f) {
            thread = std::make_unique<std::thread>(f, std::ref(*this));
            //f(*this);
        }

        friend class TransferManager;

    public:
        TransferStatus getStatus() const {
            return status;
        }

        void setStatus(TransferStatus status) {
            LocalTransferDescriptor::status = status;
        }

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
            ///@todo kill the thread or something.
            if (thread != nullptr && thread->joinable()) {
                thread->join();
            }
        }

        void wait() {
            if (thread != nullptr && thread->joinable()) {
                thread->join();
            }
        }
    };

public:
    typedef std::shared_ptr<LocalTransferDescriptor> LocalTransferDescriptorPtr;

private:
    TransferId generateTransferId();

    std::map<TransferId, std::shared_ptr<RemoteTransferDescriptor>> transfers;
    std::list<LocalTransferDescriptorPtr> localTransfers;

public:
    void beginTransfer(const TransferEvent &event);
    void transferError(const TransferEvent &event) {
///@todo
    }

    void finishTransfer(const TransferEvent &event);

    void sendData(const TransferEvent &event);

    void transferProperties(const TransferEvent &event);

    static void
    saveDataChunk(std::shared_ptr<std::ostream> outputStream, const TransferSize &begin, const TransferSize &end,
                  const RawDataType &data);


    [[nodiscard]]  LocalTransferDescriptorPtr
    initiateTransfer(const NodeIdType &nodeId, ResourceIdentificatorPtr source, ResourceIdentificatorPtr destination);

};


#endif //BASYCO_TRANSFERMANAGER_H
