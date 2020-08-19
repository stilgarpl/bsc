//
// Created by Krzysztof Tulidowicz on 07.11.17.
//

#ifndef BSC_IDEPENDENCY_H
#define BSC_IDEPENDENCY_H

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


#endif //BSC_IDEPENDENCY_H
