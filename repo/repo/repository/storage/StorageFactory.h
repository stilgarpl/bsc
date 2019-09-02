//
// Created by stilgar on 19.08.2019.
//

#ifndef BASYCO_STORAGEFACTORY_H
#define BASYCO_STORAGEFACTORY_H


#include <core/factory/Factory.h>
#include "IStorage.h"
#include "StorageFactorySpecialization.h"


class StorageFactory : public Factory<IStoragePtr,StorageFactoryByType>{
private:
    fs::path storagePath;
public:

    IStoragePtr create(const SelectorType &selector, const ArgumentType &argument) override;

    explicit StorageFactory(const fs::path &storagePath);

};


#endif //BASYCO_STORAGEFACTORY_H
