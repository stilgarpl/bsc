//
// Created by stilgar on 31.12.17.
//

#include <logic/context/LogicContext.h>
#include "DataTransfer.h"

bsc::TransferId bsc::DataTransfer::Request::getTransferId() const {
    return transferId;
}

void bsc::DataTransfer::Request::setTransferId(TransferId transferId) {
    Request::transferId = transferId;
}

bsc::TransferSize bsc::DataTransfer::Request::getBegin() const {
    return begin;
}

void bsc::DataTransfer::Request::setBegin(TransferSize begin) {
    Request::begin = begin;
}

bsc::TransferSize bsc::DataTransfer::Request::getEnd() const {
    return end;
}

void bsc::DataTransfer::Request::setEnd(TransferSize end) {
    Request::end = end;
}
