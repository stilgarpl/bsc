//
// Created by stilgar on 31.12.17.
//

#include <p2p/modules/filesystem/network/logic/sources/TransferSource.h>
#include <logic/context/LogicContext.h>
#include "DataTransfer.h"

void DataTransfer::Request::process(Context::Ptr context) {
    BasePacket::process(context);
    LogicContext::getLogicManagerFromActiveContext().getSource<TransferSource>()->sendData(this->getTransferId(),
                                                                                           this->getBegin(),
                                                                                           this->getEnd());

}

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
