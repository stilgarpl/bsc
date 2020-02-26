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
    protected:
        //@todo maybe I should remove repository refefence? strategy packs could then be global and constant. on the other hand, repository would have to be passed everytime to strategy, clouding the interface. 2020-02-26 I really think this should be done, manipulator passed as argument. do it when done with refactor
        RepositoryManipulator& manipulator;

    public:
        DeployState apply(const fs::path& path) {
            return this->apply(path, std::nullopt);
        }

        //right now it returns deployed state, but maybe it should return enum or sth //@todo think about it
        virtual DeployState apply(const fs::path& path, const std::optional<RepositoryAttributes>& attributes) = 0;

        RepositoryActionStrategy(RepositoryManipulator& manipulator) : manipulator(manipulator) {}
    };

}// namespace bsc

#endif