//
// Created by stilgar on 07.11.17.
//

#ifndef BASYCO_IDEPENDENCYMANAGED_H
#define BASYCO_IDEPENDENCYMANAGED_H

#include "IDependency.h"
#include <memory>

namespace bsc {
    class IDependencyManaged {
    public:
        virtual IDependency::TypeIdType getDependencyId() = 0;

        virtual const std::shared_ptr<IDependency>& getRequiredDependencies() const = 0;
    };
}


#endif //BASYCO_IDEPENDENCYMANAGED_H
