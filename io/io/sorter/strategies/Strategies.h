//
// Created by Krzysztof Tulidowicz on 08.06.2020.
//

#ifndef BSC_STRATEGIES_H
#define BSC_STRATEGIES_H
#include <core/factory/CopyFactory.h>
#include <core/factory/ParsingFactory.h>
#include <functional>
#include <io/sorter/exceptions/FileSortingExceptions.h>
#include <io/sorter/result/SortResult.h>
namespace bsc {

    struct FileSortingStrategies {
        using SortStrategy                  = std::function<void(const fs::path& from, const fs::path& to)>;
        using FileExistsPredicate           = std::function<bool(const fs::path& target)>;
        using CreateValidTargetPathStrategy = std::function<fs::path(
                const fs::path& target,
                const FileExistsPredicate& fileExistPredicate)>;// takes path to existing file and returns @todo think
                                                                // of return type and abort() implementation. maybe it
                                                                // should return optional<path>?
        //@todo I can probably remove from, since it's included in exception now.
        using ErrorHandlingStrategy = std::function<
                void(const fs::path& from, const FileSortingException& exception, std::list<SortFailure>& sortFailure)>;
    };

    template<>
    struct FactoryTraits<FileSortingStrategies::SortStrategy, NoFactorySpecialization> {
        using SelectorType = std::string;
        using ArgumentType = std::vector<std::string>;
    };

    template<>
    struct FactoryTraits<FileSortingStrategies::ErrorHandlingStrategy, NoFactorySpecialization> {
        using SelectorType = std::string;
        using ArgumentType = std::vector<std::string>;
    };
    template<>
    struct FactoryTraits<FileSortingStrategies::CreateValidTargetPathStrategy, NoFactorySpecialization> {
        using SelectorType = std::string;
        using ArgumentType = std::vector<std::string>;
    };
    struct FileSortingStrategyFactories {
        using SortStrategyFactory = ParsingFactory<FileSortingStrategies::SortStrategy>;
        using CreateValidTargetPathStrategyFactory =
                ParsingFactory<FileSortingStrategies::CreateValidTargetPathStrategy>;
        using ErrorActionFactory = ParsingFactory<FileSortingStrategies::ErrorHandlingStrategy>;
    };

}// namespace bsc
#endif// BSC_STRATEGIES_H