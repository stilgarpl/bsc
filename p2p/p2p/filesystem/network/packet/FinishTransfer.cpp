//
// Created by stilgar on 30.12.17.
//

#include "FinishTransfer.h"
#include <p2p/network/protocol/context/LogicContext.h>
#include <p2p/filesystem/network/logic/sources/TransferSource.h>

void FinishTransfer::Request::process(Context::Ptr context) {
    BasePacket::process(context);
    LogicContext::getLogicManagerFromActiveContext().getSource<TransferSource>()->finishTransfer(this->getTransferId());
}
