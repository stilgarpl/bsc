//
// Created by Krzysztof Tulidowicz on 20.04.2020.
//

#ifndef BSC_FILESORTER_H
#define BSC_FILESORTER_H

#include <filesystem>
#include <io/sorter/fetchers/FileListFetcher.h>
#include <io/sorter/mappers/FileSorterMapper.h>
#include <io/sorter/result/SortResult.h>
#include <io/sorter/strategies/StandardFileSorterStrategies.h>
#include <io/sorter/strategies/Strategies.h>
#include <list>
namespace bsc {

    class FileSorter {
    public:


        struct SortingStrategies {
            const FileSortingStrategies::SortStrategy sortStrategy = StandardFileSorterSortStrategies::copy;
            const FileSortingStrategies::CreateValidTargetPathStrategy createValidTargetPathStrategy =
                    StandardCreateValidTargetPathStrategies::rename();
            const FileSortingStrategies::ErrorHandlingStrategy errorHandlerStrategy =
                    StandardFileSorterErrorHandlers::stop;
            const FileSortingStrategies::FileExistsPredicate fileExistsPredicate =
                    StandardFileSorterPredicates::fileExistsPredicate;
        };

    private:
        const SortingStrategies actions;
        FileSorterMapper mapper;
        const std::unique_ptr<FileListFetcher> fileListFetcher;

    public:
        FileSorter(std::unique_ptr<FileListFetcher> fileListFetcher, SortingStrategies Actions);
        SortResult sort(const fs::path& pathToSort);
        //@todo replace with better way to set up matchers and patterns:
        void addPattern(std::unique_ptr<FileSorterMapperMatcher> matcher, std::string pattern) {
            mapper.addPattern(std::move(matcher), std::move(pattern));
        }
    };
}// namespace bsc

#endif