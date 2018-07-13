//
// Created by stilgar on 30.12.17.
//

#ifndef BASYCO_TRANSFERSOURCE_H
#define BASYCO_TRANSFERSOURCE_H


#include <p2p/logic/sources/EventQueueSource.h>
#include <p2p/modules/filesystem/network/logic/events/TransferEvent.h>

class TransferSource : public EventQueueSource<TransferEvent> {

public:

    TransferSource(SourceManager &sourceManager);

    void beginTransfer(const ResourceIdentificatorPtr resourceIdentificatorPtr);

    void finishTransfer(const TransferId &transferId);

    void sendData(const TransferId &transferId, const TransferSize &begin, const TransferSize &end);

    void transferProperties(const TransferId &transferId);

};


#endif //BASYCO_TRANSFERSOURCE_H
