//
// Created by stilgar on 19.03.19.
//

#include <p2p/modules/filesystem/FilesystemModule.h>
#include <p2p/node/context/NodeContext.h>
#include "FilesystemNetworkProcessors.h"

//@todo change into ordinary static func or sth. move it as lambda to module (as other lambdas are)
std::function<ResponsePtr<TransferQuery>(RequestPtr<TransferQuery>)> FilesystemNetworkProcessors::queryProcessor = [](
        const TransferQuery::Request::Ptr& ptr) {
//    LOGGER("Transfer Query Processor")
    auto module = NodeContext::getNodeFromActiveContext().getModule<FilesystemModule>();
    TransferQuery::Response::Ptr res = TransferQuery::Response::getNew();

    res->setResourceIdentificator(ptr->getResourceIdentificator());
    res->setExists(ptr != nullptr && ptr->getResourceIdentificator()->exists());

    return res;
};
