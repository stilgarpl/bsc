//
// Created by Krzysztof Tulidowicz on 01.01.18.
//

#ifndef BSC_STORAGEMANIFEST_H
#define BSC_STORAGEMANIFEST_H


#include "IStorage.h"

namespace bsc {
    class StorageManifest {
    private:
        std::list<IStorage::ResourceId> resources;

    };
}

#endif //BSC_STORAGEMANIFEST_H
