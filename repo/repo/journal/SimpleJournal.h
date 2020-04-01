//
// Created by stilgar on 17.10.17.
//

#ifndef BSC_SIMPLEJOURNAL_H
#define BSC_SIMPLEJOURNAL_H

#include <core/utils/cereal_include.h>

#include "IJournal.h"
#include "JournalHistory.h"
#include "JournalMetaDataFetcher.h"
#include "JournalState.h"
#include "JournalTypes.h"

namespace bsc {


    class SimpleJournal : public IJournal {

    private:
        ChecksumType checksum;
        JournalStatePtr currentState = nullptr;
        JournalHistory journalHistory;
        //@todo I'd like this to be const... or not embedded in journal. there should be something like a service that makes journal, journal should be pure data.
        //  so move it somewhere else. When it's moved metadatafetcher won't have to be serializable
        std::unique_ptr<JournalMetaDataFetcher> metaDataFetcher;
    private:
        template<class Archive>
        void serialize(Archive& ar) {

            ar(cereal::base_class<IJournal>(this), CEREAL_NVP(checksum), CEREAL_NVP(journalHistory),
               CEREAL_NVP(metaDataFetcher));
            //currentState is not serialized
        }


        friend class cereal::access;

        std::shared_ptr<JournalState> findLastState() {
            return journalHistory.getLastState();

        }

        void checkCurrentState() {
            //@todo this is wrong. last state is commited it should be current->setPrev(last)
//        if (currentState == nullptr && journalHistory.size() > 0) {
//            currentState = findLastState();
//        }
        }

        void prepareState();

    public:
        [[nodiscard]] ChecksumType getChecksum() const override;

        void commitState(CommitTimeType now) override;


        //@todo hmm hmm variadic template? - moved may have two paths for example?
        void append(JournalMethod method, JournalTarget target, PathType path, bsc::FileData data) override;

        void replay(const JournalFuncMap& funcMap) const override;

        const std::string& calculateChecksum();

        //@todo debug method, remove eventually
        void printHistory() override {
            journalHistory.printHistory();
        }




        bool merge(std::shared_ptr<SimpleJournal> other);

        bool merge(const JournalPtr& other) override;


        void replayCurrentState(const JournalFuncMap&  funcMap) override;

        JournalStatePtr getState(const CommitTimeType& commitTime, const ChecksumType& checksumType) override;

        ~SimpleJournal() override = default;

        SimpleJournal(std::unique_ptr<JournalMetaDataFetcher> metaDataFetcher);

        SimpleJournal();

    };
}
CEREAL_REGISTER_TYPE(bsc::SimpleJournal)

CEREAL_REGISTER_POLYMORPHIC_RELATION(bsc::IJournal, bsc::SimpleJournal)

#endif //BSC_SIMPLEJOURNAL_H
