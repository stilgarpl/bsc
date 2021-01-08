//
// Created by Krzysztof Tulidowicz on 20.04.2020.
//

#ifndef BSC_STANDARDFILESORTERSTRATEGIES_H
#define BSC_STANDARDFILESORTERSTRATEGIES_H
#include <io/sorter/strategies/Strategies.h>
#include <regex>
namespace bsc {
    struct StandardFileSorterSortStrategies {
        static FileSortingStrategies::SortStrategy copy;
        static FileSortingStrategies::SortStrategy move;
        static FileSortingStrategies::SortStrategy erase;
        static FileSortingStrategies::SortStrategy pretend;
    };

    struct StandardCreateValidTargetPathStrategies {
        static FileSortingStrategies::CreateValidTargetPathStrategy overwrite;
        static FileSortingStrategies::CreateValidTargetPathStrategy abort;
        static FileSortingStrategies::CreateValidTargetPathStrategy skip;
        static FileSortingStrategies::CreateValidTargetPathStrategy rename(const std::string& target = " ({})");
    };

    struct StandardFileSorterErrorHandlers {
        static FileSortingStrategies::ErrorHandlingStrategy ignore;
        static FileSortingStrategies::ErrorHandlingStrategy logAndContinue;
        static FileSortingStrategies::ErrorHandlingStrategy stop;
    };

    struct StandardFileSorterPredicates {
        static FileSortingStrategies::FileExistsPredicate fileExistsPredicate;
        static FileSortingStrategies::FileExistsPredicate pretendFileExistsPredicate;
    };

}// namespace bsc

#endif