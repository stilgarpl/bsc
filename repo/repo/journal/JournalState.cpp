//
// Created by stilgar on 17.10.17.
//

#include "JournalState.h"

void JournalState::add(const JournalStateData &data) {
    dataList.push_back(data);
}

const std::list<JournalStateData> &JournalState::getDataList() const {
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
    std::stringstream ss;
    std::string hash;
    {
        cereal::BinaryOutputArchive oa(ss);
        oa << *this;
    }
    CryptoPP::SHA1 sha1;
    CryptoPP::StringSource(ss.str(), true,
                           new CryptoPP::HashFilter(sha1, new CryptoPP::HexEncoder(new CryptoPP::StringSink(hash))));
    return hash;
}

const std::shared_ptr<JournalState> &JournalState::getPreviousState() const {
    return previousState;
}

void JournalState::setPreviousState(const std::shared_ptr<JournalState> &previousState) {
    JournalState::previousState = previousState;
}
