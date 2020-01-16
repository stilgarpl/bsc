//
// Created by stilgar on 30.08.2019.
//

#ifndef BASYCO_STORAGEFACTORYSPECIALIZATION_H
#define BASYCO_STORAGEFACTORYSPECIALIZATION_H


#include "IStorage.h"
#include <core/factory/Factory.h>

namespace bsc {
    class StorageFactoryByName {
    };

    class StorageFactoryByType {
    };

    template<>
    struct FactoryTraits<IStoragePtr, StorageFactoryByType> {
        using SelectorType = std::string;
        using ArgumentType = std::string;
    };

}


#endif //BASYCO_STORAGEFACTORYSPECIALIZATION_H
