//
// Created by stilgar on 17.10.17.
//

#include <p2p/log/Logger.h>
#include "JournalState.h"

void JournalState::add(const JournalStateData &data) {
    dataList.push_back(data);
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

const std::chrono::system_clock::time_point &JournalState::getCommitTime() const {
    return commitTime;
}

bool JournalState::operator==(const JournalState &rhs) const {
    return checksum == rhs.checksum &&
           commitTime == rhs.commitTime;
}

bool JournalState::operator!=(const JournalState &rhs) const {
    return !(rhs == *this);
}

const ResourceId &JournalState::getChecksum() const {
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

void JournalStateData::update() {

    if (fs::exists(path) && !fs::is_directory(path)) {
        CryptoPP::SHA256 hash;
        std::string digest;
        size = fs::file_size(fs::path(path));
        modificationTime = std::chrono::system_clock::to_time_t(fs::last_write_time(path));
        permissions = fs::status(path).permissions();

        CryptoPP::FileSource f(path.c_str(), true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(digest))));
        checksum = std::move(digest);
    }
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
