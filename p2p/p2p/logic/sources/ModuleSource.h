//
// Created by stilgar on 27.10.18.
//

#ifndef BASYCO_MODULESOURCE_H
#define BASYCO_MODULESOURCE_H


#include "AutoSource.h"

class ModuleSource : public ISource {

private:
    AutoSource source;

public:
    void work() override;

    explicit ModuleSource(SourceManager &sourceManager);


};


#endif //BASYCO_MODULESOURCE_H
