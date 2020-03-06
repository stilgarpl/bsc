//
// Created by stilgar on 26.02.2020.
//

#ifndef BSC_REPOSITORYMANIPULATOR_H
#define BSC_REPOSITORYMANIPULATOR_H


#include "RepositoryFileMap.h"
#include <core/log/Logger.h>
#include <filesystem>

namespace bsc {
    namespace fs = std::filesystem;
    class IRepository;
    class RepositoryManipulator {
    private:
        IRepository& repository;

    public:
        RepositoryManipulator(IRepository& repository);

        void persist(fs::path path, const RepositoryFileMap::ValueType& attributes);
        void forget(fs::path path, const RepositoryFileMap::ValueType& attributes);
        void remove(fs::path path, const RepositoryFileMap::ValueType& attributes);
        void ignore(fs::path path, const RepositoryFileMap::ValueType& attributes);
        void restoreAttributes(const fs::path& path, const RepositoryFileMap::ValueType& attributes);
        void restoreFileFromStorage(const fs::path& path, const RepositoryFileMap::ValueType& attributes);
        void trash(const fs::path& path);
        void commit(IStorage& storage);
    };
}// namespace bsc

#endif