//
// Created by stilgar on 17.10.17.
//

#ifndef BSC_IJOURNAL_H
#define BSC_IJOURNAL_H


#include "JournalFuncMap.h"
#include "JournalMethod.h"
#include "JournalState.h"
#include "JournalTypes.h"
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>


namespace bsc {

    class IJournal {
    public:
    typedef std::shared_ptr<IJournal> JournalPtr;
public:
    virtual ChecksumId getChecksum() const = 0;

    virtual void commitState(CommitTimeType now) = 0;

    //@todo add parameters, commit range or sth
    virtual void replay(const JournalFuncMap& funcMap) const = 0;

    virtual void replayCurrentState(const JournalFuncMap& funcMap) = 0;

private:
    template<class Archive>
    void serialize(Archive &ar) {
    }


    friend class cereal::access;

public:
    virtual void append(JournalMethod method, JournalTarget target, PathType path, bsc::FileData data) = 0;

    virtual void printHistory()=0;

    virtual bool merge(const JournalPtr &other) =0;

        virtual JournalStatePtr getState(const CommitTimeType& commitTime, const ChecksumType& checksumType) = 0;

        virtual ~IJournal() = default;
    };

    typedef std::shared_ptr<IJournal> JournalPtr;
    typedef std::shared_ptr<const IJournal> JournalPtrConst;
}
CEREAL_REGISTER_TYPE(bsc::IJournal)

#endif //BSC_IJOURNAL_H
