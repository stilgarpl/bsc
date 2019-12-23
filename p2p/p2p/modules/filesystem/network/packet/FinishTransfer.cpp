//
// Created by stilgar on 30.12.17.
//

#include "FinishTransfer.h"
#include <logic/context/LogicContext.h>

void FinishTransfer::Request::process(Context::Ptr context) {
    //@todo remove obsolete
//    BasePacket::process(context);
//    LogicContext::getLogicManagerFromActiveContext().getSource<TransferSource>()->finishTransfer(this->getTransferId());
}
