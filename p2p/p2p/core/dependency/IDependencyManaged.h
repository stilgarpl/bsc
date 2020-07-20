//
// Created by Krzysztof Tulidowicz on 07.11.17.
//

#ifndef BSC_IDEPENDENCYMANAGED_H
#define BSC_IDEPENDENCYMANAGED_H

#include "IDependency.h"
#include <memory>

namespace bsc {
    class IDependencyManaged {
    public:
        virtual IDependency::TypeIdType getDependencyId() = 0;

        virtual const std::shared_ptr<IDependency>& getRequiredDependencies() const = 0;
    };
}


#endif //BSC_IDEPENDENCYMANAGED_H
