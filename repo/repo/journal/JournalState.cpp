//
// Created by stilgar on 17.10.17.
//
#include <Poco/SHA1Engine.h>
#include <core/log/Logger.h>
#include <Poco/Environment.h>
#include <p2p/node/context/NodeContext.h>
#include "JournalState.h"

void JournalState::add(const JournalStateData &data) {
    auto same = std::find_if(dataList.begin(), dataList.end(), [&](auto i) {
        //@todo about that method and target... shouldn't this be an error if we have more than one method on one file?
        return data.getChecksum() == i.getChecksum() && data.getSize() == i.getSize() &&
               data.getMethod() == i.getMethod() && data.getTarget() == i.getTarget() && data.getPath() == i.getPath();
    });
    if (same == dataList.end()) {
        dataList.push_back(data);
    } else {
        LOGGER("ERROR: trying to add same data again!" + data.getPath())
    }

}

std::list<JournalStateData> &JournalState::getDataList() {
    return dataList;
}

void JournalState::setDataList(const std::list<JournalStateData> &dataList) {
    JournalState::dataList = dataList;
}

void JournalState::commit(CommitTimeType now) {
    //@todo how about a way to override this during test? commit(clock=system_clock)? or maybe get the clock from context?
    commitTime = now;
    checksum = calculateChecksum();
}

std::string JournalState::calculateChecksum() {
    //@todo make sure this is deterministic. instead of using cereal to get the string, just generate it yourself.
    checksum = "";
    std::stringstream ss;
    std::string hash;
    {
        cereal::PortableBinaryOutputArchive oa(ss);
        oa << *this;
    }

    Poco::SHA1Engine sha1Engine;

    sha1Engine.update(ss.str());
    hash = Poco::SHA1Engine::digestToHex(sha1Engine.digest());

    // LOGGER("calculated hash " + hash);
    checksum = hash;
    return hash;
}

const std::shared_ptr<JournalState> &JournalState::getPreviousState() const {
    return previousState;
}

void JournalState::setPreviousState(const std::shared_ptr<JournalState> &previousState) {
    JournalState::previousState = previousState;
}

const CommitTimeType &JournalState::getCommitTime() const {
    return commitTime;
}

bool JournalState::operator==(const JournalState &rhs) const {
    return checksum == rhs.checksum &&
           commitTime == rhs.commitTime;
}

bool JournalState::operator!=(const JournalState &rhs) const {
    return !(rhs == *this);
}

const ChecksumType &JournalState::getChecksum() const {
    return checksum;
}

bool JournalState::isProcessed() const {
    bool retVal = true;
    for (const auto &item : dataList) {
        retVal &= item.isProcessed();
    }
    return retVal;
}

void JournalState::clearProcessed() {
    for (auto &item : dataList) {
        item.setProcessed(false);
    }

}

void JournalStateData::update(FileData data) {

    // if (!data.isIsDirectory()) {
        size = data.getSize();
        modificationTime = data.getModificationTime();
        permissions = data.getPermissions();
        checksum = data.getSha256hash();
    directory = data.isIsDirectory();
    //}
}

fs::perms JournalStateData::getPermissions() const {
    return permissions;
}

bool JournalStateData::isProcessed() const {
    return processed;
}

void JournalStateData::setProcessed(bool processed) {
    JournalStateData::processed = processed;
}

bool JournalStateData::isDirectory() const {
    return directory;
}

JournalTarget JournalStateData::getTarget() const {
    return target;
}

const std::string &JournalMetaData::getNodeId() const {
    return nodeId;
}

const std::string &JournalMetaData::getUserId() const {
    return userId;
}

const std::string &JournalMetaData::getOperatingSystem() const {
    return operatingSystem;
}

JournalMetaData::JournalMetaData() {

    Poco::Environment env;
    operatingSystem = env.osName();
    //@todo implement to work on all platforms
    userId = getenv("USER");
    if (Context::getActiveContext() != nullptr) {
        nodeId = NodeContext::getNodeFromActiveContext().getNodeInfo().getNodeId();
    }
}

