//
// Created by stilgar on 30.12.17.
//

#include "FinishTransfer.h"
#include <p2p/node/context/LogicContext.h>
#include <p2p/modules/filesystem/network/logic/sources/TransferSource.h>

void FinishTransfer::Request::process(Context::Ptr context) {
    BasePacket::process(context);
    LogicContext::getLogicManagerFromActiveContext().getSource<TransferSource>()->finishTransfer(this->getTransferId());
}
