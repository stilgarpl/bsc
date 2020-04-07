//
// Created by stilgar on 17.10.17.
//

#ifndef BSC_JOURNAL_H
#define BSC_JOURNAL_H


#include "JournalFuncMap.h"
#include "JournalMethod.h"
#include "JournalState.h"
#include "JournalTypes.h"
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>


namespace bsc {

    class Journal {
    public:
    typedef std::shared_ptr<Journal> JournalPtr;
public:
    virtual ChecksumId getChecksum() const = 0;

    virtual void commitState(CommitTimeType now) = 0;

    //@todo add parameters, commit range or sth
    virtual void replay(const JournalFuncMap& funcMap) const = 0;

    virtual void replayCurrentState(const JournalFuncMap& funcMap) = 0;

protected:
    virtual JournalStatePtr& getCurrentState() = 0;

private:
    template<class Archive>
    void serialize(Archive &ar) {
    }


    friend class cereal::access;

public:
    virtual void append(JournalMethod method, JournalTarget target, PathType path, bsc::FileData data) = 0;

        template<JournalTarget target>
        void appendState(JournalStateEntry<target> data){
            getCurrentState()->add(data);
        }

    virtual void printHistory()=0;

    virtual bool merge(const JournalPtr &other) =0;

        virtual JournalStatePtr getState(const CommitTimeType& commitTime, const ChecksumType& checksumType) = 0;

        virtual ~Journal() = default;
    };

    typedef std::shared_ptr<Journal> JournalPtr;
    typedef std::shared_ptr<const Journal> JournalPtrConst;
}
CEREAL_REGISTER_TYPE(bsc::Journal)

#endif//BSC_JOURNAL_H
