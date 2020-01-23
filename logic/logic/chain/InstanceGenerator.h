//
// Created by stilgar on 21.03.19.
//

#ifndef BSC_INSTANCEGENERATOR_H
#define BSC_INSTANCEGENERATOR_H

#include <mutex>

namespace bsc {
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
}


#endif //BSC_INSTANCEGENERATOR_H
