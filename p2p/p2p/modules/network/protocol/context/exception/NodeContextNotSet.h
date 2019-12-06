//
// Created by stilgar on 11.12.17.
//

#ifndef BASYCO_NODECONTEXTNOTSET_H
#define BASYCO_NODECONTEXTNOTSET_H


#include <stdexcept>

class NodeContextNotSet : public std::domain_error {

public:
    NodeContextNotSet(const std::string& arg);
};


#endif //BASYCO_NODECONTEXTNOTSET_H
