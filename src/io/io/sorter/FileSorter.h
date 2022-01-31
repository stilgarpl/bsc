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
        struct GlobalSortingStrategies {
            const FileSortingStrategies::FileExistsPredicate fileExistsPredicate    = StandardFileSorterPredicates::fileExistsPredicate;
            const FileSortingStrategies::RelativePathBuilder relativePathBuilder = StandardRelativePathBuilders::none;
        };

    private:
        const GlobalSortingStrategies actions;
        FileSorterMapper mapper;
        const FileListFetcher fileListFetcher;
        const PathTranslator translator{};
        const FileInfoDecoder decoder{};

    public:
        FileSorter(FileListFetcher fileListFetcher, GlobalSortingStrategies Actions);
        //@todo consider replacing vector with span - it's not trivial, many places have {} initialization or just won't convert vector to
        //span (temporaries)
        SortResult sort(const std::vector<fs::path>& pathToSort);
        void addPattern(std::vector<FileSorterMapperMatcher> matcher, std::string pattern, FileSortingStrategies strategies) {
            mapper.addPattern(std::move(matcher), std::move(pattern), strategies);
        }

    private:
        [[nodiscard]] std::vector<fs::path> fetchAllFiles(const std::vector<fs::path>& pathsToSort) const;
    };
}// namespace bsc

#endif