//
// Created by stilgar on 30.12.17.
//

#ifndef BASYCO_TRANSFEREVENT_H
#define BASYCO_TRANSFEREVENT_H

//enum class TransferEventStatus {
//    STARTED,
//};

#include <p2p/modules/network/protocol/logic/events/NetworkingEvent.h>
#include <p2p/modules/filesystem/identification/TransferTypes.h>
#include <p2p/modules/filesystem/identification/ResourceIdentificator.h>

class TransferEvent : public NetworkingEvent<TransferStatus> {

private:
    TransferId transferId;
    ResourceIdentificatorPtr resourceIdentificator;
    TransferSize begin, end;

public:
    TransferId getTransferId() const;

    void setTransferId(TransferId transferId);

    const ResourceIdentificatorPtr &getResourceIdentificator() const;

    void setResourceIdentificator(const ResourceIdentificatorPtr &resourceIdentificator);

    TransferSize getBegin() const;

    void setBegin(TransferSize begin);

    TransferSize getEnd() const;

    void setEnd(TransferSize end);


};


#endif //BASYCO_TRANSFEREVENT_H
