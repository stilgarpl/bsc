//
// Created by stilgar on 26.08.17.
//

#ifndef BASYCO_NETWORKSOURCE_H
#define BASYCO_NETWORKSOURCE_H


#include "../../../logic/ISource.h"


class NetworkSource : public ISource {
public:
    void work() override;

    void registerProviders(SourceManager *manager) override;
};


#endif //BASYCO_NETWORKSOURCE_H
