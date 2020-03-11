//
// Created by stilgar on 17.10.17.
//
#include <core/log/Logger.h>
#include <Poco/Environment.h>
#include <core/utils/crypto.h>

#include <utility>
#include "JournalState.h"
namespace bsc {


    void JournalState::commit(CommitTimeType now) {
        //@todo how about a way to override this during test? commit(Clock=system_clock)? or maybe get the Clock from context?
        commitTime = now;
        checksum = calculateChecksum();
    }

    std::string JournalState::calculateChecksum() {
        //@todo make sure this is deterministic. instead of using cereal to get the string, just generate it yourself.
        checksum = "";
        std::stringstream ss;
        {
            using namespace std::chrono;
            //@todo add journal utils that will do these conversions to strings and checksums
            ss << metaData.getChecksum()
               << std::to_string(duration_cast<seconds>(commitTime.time_since_epoch()).count());
            //@todo this assumes dataList is sorted.
            for (const auto& data : dataList) {
                std::string dataChecksum;
                std::visit([&dataChecksum](auto i){dataChecksum = i.calculateChecksum();},data);
                ss << dataChecksum;
            }
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