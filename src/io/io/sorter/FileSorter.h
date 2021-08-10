//
// Created by Krzysztof Tulidowicz on 20.04.2020.
//

#ifndef BSC_FILESORTER_H
#define BSC_FILESORTER_H

#include <filesystem>
#include <io/file/FileInfoDecoder.h>
#include <io/sorter/fetchers/FileListFetcher.h>
#include <io/sorter/mappers/FileSorterMapper.h>
#include <io/sorter/result/SortResult.h>
#include <io/sorter/strategies/StandardFileSorterStrategies.h>
#include <io/sorter/strategies/Strategies.h>
#include <io/translator/PathTranslator.h>
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
            const FileSortingStrategies::FileExistsPredicate fileExistsPredicate = StandardFileSorterPredicates::fileExistsPredicate;
        };

    private:
        const SortingStrategies actions;
        FileSorterMapper mapper;
        const FileListFetcher fileListFetcher;
        const PathTranslator translator{};
        const FileInfoDecoder decoder{};

    public:
        FileSorter(FileListFetcher fileListFetcher, SortingStrategies Actions);
        //@todo consider replacing vector with span - it's not trivial, many places have {} initialization or just won't convert vector to span (temporaries)
        SortResult sort(const std::vector<fs::path>& pathToSort);
        void addPattern(FileSorterMapperMatcher matcher, std::string pattern) {
            mapper.addPattern(std::move(matcher), std::move(pattern));
        }
    private:
        [[nodiscard]] std::vector<fs::path> fetchAllFiles(const std::vector<fs::path>& pathsToSort) const;
    };
}// namespace bsc

#endif