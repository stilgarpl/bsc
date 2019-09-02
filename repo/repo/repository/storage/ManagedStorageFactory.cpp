//
// Created by stilgar on 30.08.2019.
//

#include "ManagedStorageFactory.h"
#include "InternalStorage.h"

//IStoragePtr ManagedStorageFactory::create(const int &selector) {
//    return IStoragePtr();
//}

IStoragePtr ManagedStorageFactory::create(const SelectorType &selector) {
    //@todo throw if null? or not?
    return manager.findStorage(selector);
}

ManagedStorageFactory::ManagedStorageFactory(StorageManager &manager) : manager(manager) {}
