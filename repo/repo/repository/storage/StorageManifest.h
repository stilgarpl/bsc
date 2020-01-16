//
// Created by stilgar on 01.01.18.
//

#ifndef BASYCO_STORAGEMANIFEST_H
#define BASYCO_STORAGEMANIFEST_H


#include "IStorage.h"
namespace bsc {
    class StorageManifest {
    private:
        std::list<IStorage::ResourceId> resources;

    };
}

#endif //BASYCO_STORAGEMANIFEST_H
