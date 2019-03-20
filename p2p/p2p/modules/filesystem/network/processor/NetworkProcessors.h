//
// Created by stilgar on 19.03.19.
//

#ifndef BASYCO_NETWORKPROCESSORS_H
#define BASYCO_NETWORKPROCESSORS_H

#include <p2p/modules/filesystem/network/packet/TransferQuery.h>

struct NetworkProcessors {

    static std::function<TransferQuery::Response::Ptr(TransferQuery::Request::Ptr)> queryProcessor;
};


#endif //BASYCO_NETWORKPROCESSORS_H
