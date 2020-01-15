//
// Created by stilgar on 21.11.2019.
//

#include "JournalState.h"
#include <core/utils/crypto.h>

#include <utility>
#include "JournalMetaData.h"

const std::string& JournalMetaData::getNodeId() const {
    return nodeId;
}

const std::string& JournalMetaData::getUserId() const {
    return userId;
}

const std::string& JournalMetaData::getOperatingSystem() const {
    return operatingSystem;
}

ChecksumType JournalMetaData::getChecksum() {
    std::stringstream ss;
    ss << nodeId << userId << operatingSystem;
    return bsc::calculateSha1OfString(ss.str());
}

JournalMetaData::JournalMetaData(NodeIdType nodeId, std::string userId,
                                 std::string operatingSystem) : nodeId(std::move(nodeId)), userId(std::move(userId)),
                                                                operatingSystem(std::move(operatingSystem)) {}
