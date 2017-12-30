//
// Created by stilgar on 30.12.17.
//

#include "TransferSource.h"

void TransferSource::beginTransfer(const ResourceIdentificatorPtr resourceIdentificatorPtr) {
    auto event = newEvent();
    event->setResourceIdentificator(resourceIdentificatorPtr);
    event->setEventId(EventIdType::REQUESTED);
    queueEvent(event);
}

void TransferSource::finishTransfer(const TransferId &transferId) {
    auto event = newEvent();
    event->setTransferId(transferId);
    event->setEventId(EventIdType::FINISHING);
    queueEvent(event);
}

void TransferSource::sendData(const TransferId &transferId, const TransferSize &begin, const TransferSize &end) {
    auto event = newEvent();
    event->setTransferId(transferId);
    event->setEventId(EventIdType::SENDING);
    event->setBegin(begin);
    event->setEnd(end);
    queueEvent(event);
}

void TransferSource::getTransferProperties(const TransferId &transferId) {
    auto event = newEvent();
    event->setTransferId(transferId);
    event->setEventId(EventIdType::GET_PROPERTIES);
    queueEvent(event);
}
