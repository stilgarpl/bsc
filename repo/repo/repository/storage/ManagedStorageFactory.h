//
// Created by stilgar on 30.08.2019.
//

#ifndef BASYCO_MANAGEDSTORAGEFACTORY_H
#define BASYCO_MANAGEDSTORAGEFACTORY_H

#include "IStorage.h"
#include "StorageFactorySpecialization.h"
#include "StorageManager.h"
#include <core/factory/Factory.h>



class ManagedStorageFactory : public Factory<IStoragePtr,StorageFactoryByName> {
private:
    StorageManager &manager;
public:
//    IStoragePtr create(const SelectorType &selector) override;
    IStoragePtr create(const SelectorType &selector) override;

    explicit ManagedStorageFactory(StorageManager &manager);
};


#endif //BASYCO_MANAGEDSTORAGEFACTORY_H
