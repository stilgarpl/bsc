//
// Created by stilgar on 06.12.17.
//

#ifndef BASYCO_DIRECTEDJOURNAL_H
#define BASYCO_DIRECTEDJOURNAL_H


#include "IJournal.h"

class DirectedJournal : public IJournal {
public:
    JournalChecksumType getChecksum() override;

    void commitState() override;

    void replay() override;

    void append(JournalMethod method, JournalStateData::PathType path) override;

    void printHistory() override;
};


#endif //BASYCO_DIRECTEDJOURNAL_H
