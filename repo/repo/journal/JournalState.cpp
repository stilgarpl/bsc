//
// Created by stilgar on 17.10.17.
//
#include <Poco/Environment.h>
#include <core/log/Logger.h>
#include <core/utils/crypto.h>

#include "JournalState.h"
#include <utility>
namespace bsc {


    //@todo change the name of this func, it doesn't really commit anything, maybe call it close(), finish(), finalize() or sth.
    void JournalState::commit(CommitTimeType now,ChecksumType journalChecksum) {
        commitTime = now;
        journalChecksum = journalChecksum;
        checksum = calculateChecksum();
    }

    std::string JournalState::calculateChecksum() {
        checksum = "";
        std::stringstream ss;
        {
            using namespace std::chrono;
            //@todo add journal utils that will do these conversions to strings and checksums
            ss << metaData.getChecksum()
               << std::to_string(duration_cast<milliseconds>(commitTime.time_since_epoch()).count());
            //@todo this assumes dataList is always in the same order (sorted by time?).
            for (const auto& data : dataList) {
                ss << std::visit([](auto i) { return  i.calculateChecksum(); }, data);
            }
            ss << journalChecksum;
            ss << (previousState ? previousState->calculateChecksum() : "");
        }

        checksum = bsc::calculateSha1OfString(ss.str());

        return checksum;
    }

    const std::shared_ptr<JournalState>& JournalState::getPreviousState() const {
        return previousState;
    }

    void JournalState::setPreviousState(const std::shared_ptr<JournalState>& previousState) {
        JournalState::previousState = previousState;
    }

    const CommitTimeType& JournalState::getCommitTime() const {
        return commitTime;
    }

    bool JournalState::operator==(const JournalState& rhs) const {
        return checksum == rhs.checksum &&
               commitTime == rhs.commitTime;
    }

    bool JournalState::operator!=(const JournalState& rhs) const {
        return !(rhs == *this);
    }

    const ChecksumType& JournalState::getChecksum() const {
        return checksum;
    }

    JournalState::JournalState(JournalMetaData metaData) : metaData(std::move(metaData)) {}


}// namespace bsc