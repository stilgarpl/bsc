//
// Created by stilgar on 22.12.17.
//

#include "StorageQuery.h"

void StorageQuery::Request::process(Context &context) {
    BasePacket::process(context);

    auto storageSource = LogicContext::getLogicManagerFromActiveContext().getSource<StorageSource>();

    if (storageSource != nullptr) {
        storageSource->queryResource(repositoryId, objectId, this->getId());
    }
}
