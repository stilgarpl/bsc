//
// Created by stilgar on 17.10.17.
//

#ifndef BASYCO_IJOURNAL_H
#define BASYCO_IJOURNAL_H


#include "JournalMethod.h"
#include "JournalTypes.h"
#include "JournalState.h"
#include <map>
#include <vector>
#include <optional>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>


class IJournal {
public:

public:
    typedef std::function<void(const JournalStateData &)> Func;
    typedef std::map<std::pair<JournalMethod, JournalTarget>, std::optional<Func>> FuncMap;
    //@todo I'm not sure this should be in the interface...
    typedef std::shared_ptr<JournalState> JournalStatePtr;
    typedef std::vector<JournalStatePtr> JournalHistory;
    typedef std::shared_ptr<IJournal> JournalPtr;
public:
    virtual ResourceId getChecksum() const = 0;

    virtual void commitState() =0;

    //@todo add parameters, commit range or sth
    virtual void replay() = 0;

    virtual void replayCurrentState() = 0;

private:
    template<class Archive>
    void serialize(Archive &ar) {
    }


    friend class cereal::access;

public:
    virtual void append(JournalMethod method, JournalTarget target, PathType path, FileData data) = 0;

    virtual void printHistory()=0;

    virtual bool merge(const JournalPtr &other) =0;

    virtual void setFunc(const JournalMethod &method, const JournalTarget &target, Func func) = 0;

    virtual void clearFunc() = 0;

    virtual JournalStatePtr getState(const CommitTimeType &commitTime, const ChecksumType &checksumType) = 0;
};

typedef std::shared_ptr<IJournal> JournalPtr;
typedef std::shared_ptr<const IJournal> JournalPtrConst;

CEREAL_REGISTER_TYPE(IJournal);

#endif //BASYCO_IJOURNAL_H
