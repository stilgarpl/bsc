//
// Created by stilgar on 30.12.17.
//

#ifndef BASYCO_TRANSFERMANAGER_H
#define BASYCO_TRANSFERMANAGER_H


#include <p2p/filesystem/network/logic/events/TransferEvent.h>
#include <p2p/filesystem/network/packet/BeginTransfer.h>

class TransferManager {
    class TransferDescriptor {
//    ResourceIdentificatorPtr resourceIdentificatorPtr;
        std::shared_ptr<std::istream> inputStream;

    public:
//    const ResourceIdentificatorPtr &getResourceIdentificatorPtr() const;
//
//    void setResourceIdentificatorPtr(const ResourceIdentificatorPtr &resourceIdentificatorPtr);

        const std::shared_ptr<std::istream> &getInputStream() const;

        void setInputStream(const std::shared_ptr<std::istream> &inputStream);

    };

    std::map<TransferId, std::shared_ptr<TransferDescriptor>> transfers;
public:

    void beginTransfer(const TransferEvent &event);

    void transferError(const TransferEvent &event) {
///@todo
    }

};


#endif //BASYCO_TRANSFERMANAGER_H
