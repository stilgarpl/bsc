//
// Created by stilgar on 31.12.17.
//

#include <logic/context/LogicContext.h>
#include "DataTransfer.h"

TransferId DataTransfer::Request::getTransferId() const {
    return transferId;
}

void DataTransfer::Request::setTransferId(TransferId transferId) {
    Request::transferId = transferId;
}

TransferSize DataTransfer::Request::getBegin() const {
    return begin;
}

void DataTransfer::Request::setBegin(TransferSize begin) {
    Request::begin = begin;
}

TransferSize DataTransfer::Request::getEnd() const {
    return end;
}

void DataTransfer::Request::setEnd(TransferSize end) {
    Request::end = end;
}
