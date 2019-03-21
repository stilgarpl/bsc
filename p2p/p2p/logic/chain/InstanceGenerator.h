//
// Created by stilgar on 21.03.19.
//

#ifndef BASYCO_INSTANCEGENERATOR_H
#define BASYCO_INSTANCEGENERATOR_H

#include <mutex>

//@todo common generator interface for all generators
class InstanceGenerator {
public:
    //@todo name clash with another InstanceType defined somewhere else. fix it.
    using InstanceType = unsigned long;

private:
    mutable std::mutex valueLock;
public:
    InstanceType nextValue();
};


#endif //BASYCO_INSTANCEGENERATOR_H
