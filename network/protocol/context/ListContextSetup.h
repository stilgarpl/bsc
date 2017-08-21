//
// Created by stilgar on 21.08.17.
//

#ifndef BASYCO_LISTCONTEXTSETUP_H
#define BASYCO_LISTCONTEXTSETUP_H


#include <memory>
#include <list>
#include "IContextSetup.h"

class ListContextSetup : public IContextSetup {
    std::list<std::shared_ptr<IContextSetup>> setupList;

public:
    void setup(Context &context) override;

    void addSetup(std::shared_ptr<IContextSetup> setup);
};


#endif //BASYCO_LISTCONTEXTSETUP_H
