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
#include <experimental/optional>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>


class IJournal {
public:

public:
    typedef std::function<void(const JournalStateData &)> Func;
    typedef std::map<JournalMethod, std::optional<Func>> FuncMap;
    ///@todo I'm not sure this should be in the interface...
    typedef std::shared_ptr<JournalState> JournalStatePtr;
    typedef std::vector<std::shared_ptr<JournalState>> JournalHistory;
    typedef std::shared_ptr<IJournal> JournalPtr;
public:
    virtual ResourceId getChecksum() =0;

    virtual void commitState() =0;

    ///@todo add parameters, commit range or sth
    virtual void replay() = 0;

private:
    template<class Archive>
    void serialize(Archive &ar) {
    }


    friend class cereal::access;

public:
    virtual void append(JournalMethod method, PathType path) = 0;

    virtual void printHistory()=0;

    virtual bool merge(const JournalPtr &other) =0;

    virtual void setFunc(const JournalMethod &method, Func func) =0;

};

typedef std::shared_ptr<IJournal> JournalPtr;

CEREAL_REGISTER_TYPE(IJournal);

#endif //BASYCO_IJOURNAL_H
