//
// Created by stilgar on 20.04.2020.
//

#ifndef BSC_FILESORTER_H
#define BSC_FILESORTER_H
#include <filesystem>
#include <io/sorter/actions/SortAction.h>
#include <io/sorter/fetchers/FileListFetcher.h>
#include <io/sorter/mappers/FileSorterMapper.h>
namespace bsc {

    class FileSorter {
        const std::unique_ptr<SortAction> sortAction;
        FileSorterMapper mapper;
        const std::unique_ptr<FileListFetcher> fileListFetcher;

    public:
        FileSorter(std::unique_ptr<FileListFetcher> fileListFetcher, std::unique_ptr<SortAction> sortAction);
        void sort(const fs::path& pathToSort);
    };
}// namespace bsc

#endif