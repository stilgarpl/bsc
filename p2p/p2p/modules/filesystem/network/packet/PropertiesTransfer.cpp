//
// Created by stilgar on 31.12.17.
//

#include <p2p/node/context/LogicContext.h>
#include <p2p/modules/filesystem/network/logic/sources/TransferSource.h>
#include "PropertiesTransfer.h"

void PropertiesTransfer::Request::process(Context::Ptr context) {
    BasePacket::process(context);
    LogicContext::getLogicManagerFromActiveContext().getSource<TransferSource>()->transferProperties(
            this->getTransferId());

}

TransferId PropertiesTransfer::Request::getTransferId() const {
    return transferId;
}

void PropertiesTransfer::Request::setTransferId(TransferId transferId) {
    Request::transferId = transferId;
}
