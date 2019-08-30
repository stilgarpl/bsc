//
// Created by stilgar on 30.08.2019.
//

#ifndef BASYCO_MANAGEDSTORAGEFACTORY_H
#define BASYCO_MANAGEDSTORAGEFACTORY_H

#include "IStorage.h"
#include "StorageFactorySpecialization.h"
#include <core/factory/Factory.h>

template<>
struct FactoryTraits<IStoragePtr,StorageFactoryByName> {
    using SelectorType = int;//IStorage::StorageId ;
};

class ManagedStorageFactory : public Factory<IStoragePtr,StorageFactoryByName> {
public:
    IStoragePtr create(const SelectorType &selector) override;
};


#endif //BASYCO_MANAGEDSTORAGEFACTORY_H
