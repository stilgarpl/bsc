//
// Created by stilgar on 26.02.2020.
//

#ifndef BSC_REPOSITORYACTIONSTRATEGY_H
#define BSC_REPOSITORYACTIONSTRATEGY_H

#include "DeployState.h"
#include <filesystem>
#include <optional>
#include <repo/repository/RepositoryAttributes.h>
#include <repo/repository/RepositoryManipulator.h>

namespace bsc {
    namespace fs = std::filesystem;
    class IRepository;
    class RepositoryActionStrategy {
    public:
        DeployState apply(const fs::path& path, RepositoryManipulator& manipulator) {
            return this->apply(path, std::nullopt, manipulator);
        }

        //right now it returns deployed state, but maybe it should return enum or sth //@todo think about it
        virtual DeployState apply(const fs::path& path, const std::optional<RepositoryAttributes>& attributes, RepositoryManipulator& manipulator) = 0;
    };

}// namespace bsc

#endif