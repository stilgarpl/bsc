//
// Created by stilgar on 20.04.2020.
//

#ifndef BSC_FILESORTER_H
#define BSC_FILESORTER_H
#include <core/factory/CopyFactory.h>
#include <filesystem>
#include <io/sorter/fetchers/FileListFetcher.h>
#include <io/sorter/mappers/FileSorterMapper.h>
namespace bsc {
    //@todo handle more actions - errors like file already present -> sort action should return optional error code?
    class FileSorter {
    public:
        using SortAction   = std::function<void(const fs::path& from, const fs::path& to)>;
        using TargetAction = std::function<void(const fs::path& target)>;
        using ErrorAction  = std::function<void(const fs::path& from, const fs::path& to)>;
        //@todo I don't think factories should be defined here.
        using SortActionFactory   = CopyFactory<SortAction>;
        using TargetActionFactory = CopyFactory<TargetAction>;
        using ErrorActionFactory  = CopyFactory<ErrorAction>;
        struct Actions {
            const SortAction sortAction;
            const TargetAction targetAction;
            const ErrorAction errorAction;
        };

    private:
        const Actions actions;
        FileSorterMapper mapper;
        const std::unique_ptr<FileListFetcher> fileListFetcher;

    public:
        FileSorter(std::unique_ptr<FileListFetcher> fileListFetcher, Actions Actions);
        void sort(const fs::path& pathToSort);
        //@todo replace with better way to set up matchers and patterns:
        void addPattern(std::unique_ptr<FileSorterMapperMatcher> matcher, std::string pattern) {
            mapper.addPattern(std::move(matcher), std::move(pattern));
        }
    };
}// namespace bsc

#endif