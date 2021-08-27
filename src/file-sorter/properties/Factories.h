//
// Created by Krzysztof Tulidowicz on 27.08.2021.
//
#ifndef BSC_FACTORIES_H
#define BSC_FACTORIES_H

#include <io/sorter/strategies/Strategies.h>

namespace bsc {
    template<>
    struct FactoryTraits<FileSortingStrategies::SortStrategy, NoFactorySpecialization> {
        using SelectorType = SortAction;
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

    auto makeActionFactory() {
        static FileSortingStrategyFactories::SortStrategyFactory actionFactory;
        using enum SortAction;
        actionFactory.registerCreator(copy, StandardFileSorterSortStrategies::copy);
        actionFactory.registerCreator(move, StandardFileSorterSortStrategies::move);
        actionFactory.registerCreator(pretend, StandardFileSorterSortStrategies::pretend);
        return actionFactory;
    }

    auto makeErrorActionFactory() {
        static FileSortingStrategyFactories::ErrorActionFactory errorActionFactory;
        errorActionFactory.registerCreator("ignore", StandardFileSorterErrorHandlers::ignore);
        errorActionFactory.registerCreator("continue", StandardFileSorterErrorHandlers::logAndContinue);
        errorActionFactory.registerCreator("stop", StandardFileSorterErrorHandlers::stop);
        return errorActionFactory;
    }

    auto makeFileExistsFactory() {
        static FileSortingStrategyFactories::CreateValidTargetPathStrategyFactory fileExistsFactory;
        fileExistsFactory.registerCreator("overwrite", StandardCreateValidTargetPathStrategies::overwrite);
        fileExistsFactory.registerCreator("skip", StandardCreateValidTargetPathStrategies::skip);
        fileExistsFactory.registerCreator("abort", StandardCreateValidTargetPathStrategies::abort);
        //@todo maybe rename should be renamed to "addSuffix", because that's what it does. rename could be added as search-and-replace regex
        fileExistsFactory.registerCreator("rename", StandardCreateValidTargetPathStrategies::rename);
        return fileExistsFactory;
    }
}
#endif// BSC_FACTORIES_H
