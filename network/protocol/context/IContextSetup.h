//
// Created by stilgar on 21.08.17.
//

#ifndef BASYCO_ICONTEXTSETUP_H
#define BASYCO_ICONTEXTSETUP_H


#include "context/Context.h"

class IContextSetup {

public:
    virtual void setup(Context &context) =0;
};


#endif //BASYCO_ICONTEXTSETUP_H
