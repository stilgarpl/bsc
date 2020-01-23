//
// Created by stilgar on 19.08.2019.
//

#ifndef BSC_STORAGEFACTORY_H
#define BSC_STORAGEFACTORY_H


#include <core/factory/Factory.h>
#include "IStorage.h"
#include "StorageFactorySpecialization.h"

namespace bsc {

    class StorageFactory : public bsc::Factory<IStoragePtr, bsc::StorageFactoryByType> {
    private:
        fs::path storagePath;
    public:

        IStoragePtr create(const SelectorType& selector, const ArgumentType& argument) override;

        explicit StorageFactory(fs::path storagePath);

    };
}

#endif //BSC_STORAGEFACTORY_H
