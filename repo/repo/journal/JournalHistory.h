//
// Created by stilgar on 01.04.2020.
//

#ifndef BSC_JOURNALHISTORY_H
#define BSC_JOURNALHISTORY_H

#include "JournalState.h"
namespace bsc {
    class JournalHistory {
    private:
        std::vector<JournalStatePtr> history;

    public:
        JournalStatePtr getLastState();
        JournalStatePtr findRoot() const {
            for (auto&& item : history) {
                //@todo validator should check if there is only one root
                if (item->getPreviousState() == nullptr) {
                    return item;
                }

            }
            return nullptr;
        }
        void printHistory();
        bool merge(const JournalHistory& history);
        JournalStatePtr getState(const CommitTimeType& commitTime, const ChecksumType& checksum);
        [[nodiscard]] ChecksumType calculateChecksum();
        void insert(JournalStatePtr state);
        void replay(const JournalFuncMap& funcMap) const;

    private:
        template<class Archive>
        void serialize(Archive& ar) {
            ar(CEREAL_NVP(history));
        }


        friend class cereal::access;
    };
}// namespace bsc

#endif