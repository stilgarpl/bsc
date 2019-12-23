//
// Created by stilgar on 31.12.17.
//

#include <logic/context/LogicContext.h>
#include "PropertiesTransfer.h"

void PropertiesTransfer::Request::process(Context::Ptr context) {
    //@todo remove obsolete
//    BasePacket::process(context);
//    LogicContext::getLogicManagerFromActiveContext().getSource<TransferSource>()->transferProperties(
//            this->getTransferId());

}

TransferId PropertiesTransfer::Request::getTransferId() const {
    return transferId;
}

void PropertiesTransfer::Request::setTransferId(TransferId transferId) {
    Request::transferId = transferId;
}
