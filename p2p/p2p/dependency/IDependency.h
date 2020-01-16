//
// Created by stilgar on 07.11.17.
//

#ifndef BASYCO_IDEPENDENCY_H
#define BASYCO_IDEPENDENCY_H

#include <vector>

namespace bsc {
    class IDependency {
    public:
        typedef unsigned int TypeIdType;
        typedef std::vector<TypeIdType> ArrayType;

        virtual ArrayType getDependencyIdents() = 0;

        virtual ~IDependency() = default;
    };
}


#endif //BASYCO_IDEPENDENCY_H
