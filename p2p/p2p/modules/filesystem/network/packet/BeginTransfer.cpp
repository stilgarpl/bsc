//
// Created by stilgar on 30.12.17.
//

#include <logic/context/LogicContext.h>
#include <p2p/modules/filesystem/network/logic/sources/TransferSource.h>
#include "BeginTransfer.h"

void BeginTransfer::Request::process(Context::Ptr context) {
    BasePacket::process(context);

    LogicContext::getLogicManagerFromActiveContext().getSource<TransferSource>()->beginTransfer(this->getResourceId());

}
