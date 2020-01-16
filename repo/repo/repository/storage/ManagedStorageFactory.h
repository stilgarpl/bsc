//
// Created by stilgar on 30.08.2019.
//

#ifndef BASYCO_MANAGEDSTORAGEFACTORY_H
#define BASYCO_MANAGEDSTORAGEFACTORY_H

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
        IStoragePtr create(const SelectorType& selector) override;

        explicit ManagedStorageFactory(StorageManager& manager);
    };
}

#endif //BASYCO_MANAGEDSTORAGEFACTORY_H
