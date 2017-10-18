//
// Created by stilgar on 17.10.17.
//

#ifndef BASYCO_IJOURNAL_H
#define BASYCO_IJOURNAL_H


#include "JournalMethod.h"
#include "JournalTypes.h"
#include "JournalState.h"
#include <map>
#include <experimental/optional>

class IJournal {
public:

public:
    typedef std::function<void(const JournalStateData &)> Func;
    typedef std::map<JournalMethod, std::experimental::optional<Func>> FuncMap;
public:
    virtual JournalChecksumType getChecksum() =0;

    virtual void commitState() =0;

    ///@todo add parameters, commit range or sth
    virtual void replay() = 0;


};


#endif //BASYCO_IJOURNAL_H
