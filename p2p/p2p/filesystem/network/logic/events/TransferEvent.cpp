//
// Created by stilgar on 30.12.17.
//

#include "TransferEvent.h"

TransferId TransferEvent::getTransferId() const {
    return transferId;
}

void TransferEvent::setTransferId(TransferId transferId) {
    TransferEvent::transferId = transferId;
}

const ResourceIdentificatorPtr &TransferEvent::getResourceIdentificator() const {
    return resourceIdentificator;
}

void TransferEvent::setResourceIdentificator(const ResourceIdentificatorPtr &resourceIdentificator) {
    TransferEvent::resourceIdentificator = resourceIdentificator;
}

TransferSize TransferEvent::getBegin() const {
    return begin;
}

void TransferEvent::setBegin(TransferSize begin) {
    TransferEvent::begin = begin;
}

TransferSize TransferEvent::getEnd() const {
    return end;
}

void TransferEvent::setEnd(TransferSize end) {
    TransferEvent::end = end;
}
