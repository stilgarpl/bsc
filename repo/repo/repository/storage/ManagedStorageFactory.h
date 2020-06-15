//
// Created by stilgar on 30.08.2019.
//

#ifndef BSC_MANAGEDSTORAGEFACTORY_H
#define BSC_MANAGEDSTORAGEFACTORY_H

#include "IStorage.h"

#include "StorageManager.h"
#include "StorageFactorySpecialization.h"
#include <core/factory/Factory.h>

namespace bsc {

    class ManagedStorageFactory : public bsc::Factory<IStoragePtr, bsc::StorageFactoryByName> {
    private:
        StorageManager& manager;
    public:
        //    IStoragePtr create(const SelectorType &selector) override;
        IStoragePtr create(const SelectorType& selector) const override;

        explicit ManagedStorageFactory(StorageManager& manager);
    };
}

#endif //BSC_MANAGEDSTORAGEFACTORY_H
