//
// Created by stilgar on 22.12.17.
//

#include "StorageQuery.h"
#include <logic/context/LogicContext.h>

void StorageQuery::Request::process(Context::Ptr context) {
    BasePacket::process(context);

    auto storageSource = LogicContext::getLogicManagerFromActiveContext().getSource<StorageSource>();

    if (storageSource != nullptr) {
        storageSource->queryResource(storageId, objectId, this->getId());
    }
}
