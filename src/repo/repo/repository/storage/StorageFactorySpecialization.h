//
// Created by Krzysztof Tulidowicz on 30.08.2019.
//

#ifndef BSC_STORAGEFACTORYSPECIALIZATION_H
#define BSC_STORAGEFACTORYSPECIALIZATION_H


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


#endif //BSC_STORAGEFACTORYSPECIALIZATION_H
