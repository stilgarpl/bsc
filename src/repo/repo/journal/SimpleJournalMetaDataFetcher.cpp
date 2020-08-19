//
// Created by Krzysztof Tulidowicz on 02.12.2019.
//

#include <Poco/Environment.h>
#include <core/context/Context.h>
#include <p2p/core/node/context/NodeContext.h>
#include "SimpleJournalMetaDataFetcher.h"
namespace bsc {


    JournalMetaData SimpleJournalMetaDataFetcher::makeMetaData() {
        auto operatingSystem = Poco::Environment::osName();
        std::string userId;
        std::string nodeId;
        //@todo implement to work on all platforms
        if (auto userValue = getenv("USER"); userValue != nullptr) {
            userId = userValue;
        }
        if (Context::hasActiveContext()) {
            nodeId = bsc::NodeContext::getNodeFromActiveContext().getNodeInfo().getNodeId();
        }
        return JournalMetaData(nodeId, userId, operatingSystem);

    }
}