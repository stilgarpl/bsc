//
// Created by stilgar on 02.12.2019.
//

#include <Poco/Environment.h>
#include <core/context/Context.h>
#include <p2p/node/context/NodeContext.h>
#include "SimpleJournalMetaDataFetcher.h"



JournalMetaData SimpleJournalMetaDataFetcher::makeMetaData() {
    auto operatingSystem = Poco::Environment::osName();
    std::string userId;
    std::string nodeId;
    //@todo implement to work on all platforms
    if (auto userValue = getenv("USER"); userValue != nullptr) {
        userId = userValue;
    }
    if (bsc::Context::hasActiveContext()) {
        nodeId = bsc::NodeContext::getNodeFromActiveContext().getNodeInfo().getNodeId();
    }
    return JournalMetaData(nodeId, userId, operatingSystem);

}
