//
// Created by stilgar on 17.10.17.
//

#include <p2p/log/Logger.h>
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
        LOGGER("ERROR: trying to add same data again!")
    }

}

std::list<JournalStateData> &JournalState::getDataList() {
    return dataList;
}

void JournalState::setDataList(const std::list<JournalStateData> &dataList) {
    JournalState::dataList = dataList;
}

void JournalState::commit() {
    commitTime = std::chrono::system_clock::now();
    checksum = calculateChecksum();
//        std::stringstream ss;
//        {
//            cereal::XMLOutputArchive oa(ss);
//            oa << *this;
//        }
//
//        std::cout << ss.str() << std::endl;
}

std::string JournalState::calculateChecksum() {
    checksum = "";
    std::stringstream ss;
    std::string hash;
    {
        cereal::PortableBinaryOutputArchive oa(ss);
        oa << *this;
    }
    CryptoPP::SHA1 sha1;
    CryptoPP::StringSource(ss.str(), true,
                           new CryptoPP::HashFilter(sha1, new CryptoPP::HexEncoder(new CryptoPP::StringSink(hash))));
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
    nodeId = NodeContext::getNodeFromActiveContext().getNodeInfo().getNodeId();
}

