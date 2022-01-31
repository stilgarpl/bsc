//
// Created by Krzysztof Tulidowicz on 27.08.2021.
//
#ifndef BSC_FACTORIES_H
#define BSC_FACTORIES_H

#include "io/sorter/strategies/Strategies.h"
#include "io/sorter/strategies/StandardFileSorterStrategies.h"
#include "core/factory/FactoryPtr.h"

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

    template<>
    struct FactoryTraits<FileSortingStrategies::RelativePathBuilder, NoFactorySpecialization> {
        using SelectorType = std::string;
        using ArgumentType = std::vector<std::string>;
    };

    struct FileSortingStrategyFactories {
        using SortStrategyFactory = ParsingFactory<FileSortingStrategies::SortStrategy>;
        using CreateValidTargetPathStrategyFactory =
                ParsingFactory<FileSortingStrategies::CreateValidTargetPathStrategy>;
        using ErrorActionFactory = ParsingFactory<FileSortingStrategies::ErrorHandlingStrategy>;
        using RelativePathFactory = ParsingFactory<FileSortingStrategies::RelativePathBuilder>;
    };

    auto makeActionFactory() {
        using FactoryType = FileSortingStrategyFactories::SortStrategyFactory;
                FactoryPtr<typename FactoryType::ProducedObjectType> actionFactory;
        using enum SortAction;
        actionFactory.as<FactoryType>()->registerCreator(copy, StandardFileSorterSortStrategies::copy);
        actionFactory.as<FactoryType>()->registerCreator(move, StandardFileSorterSortStrategies::move);
        actionFactory.as<FactoryType>()->registerCreator(pretend, StandardFileSorterSortStrategies::pretend);
        return actionFactory;
    }

    auto makeRelativePathFactory() {
        using FactoryType = FileSortingStrategyFactories::RelativePathFactory;
        FactoryPtr<typename FactoryType::ProducedObjectType> relativePathFactory;
        relativePathFactory.as<FactoryType>()->registerCreator("none", StandardRelativePathBuilders::none);
        relativePathFactory.as<FactoryType>()->registerCreator("parent", StandardRelativePathBuilders::parentDirectory);
        relativePathFactory.as<FactoryType>()->registerCreator("preserve", StandardRelativePathBuilders::preserve);
        return relativePathFactory;
    }

    auto makeErrorActionFactory() {
        using FactoryType = FileSortingStrategyFactories::ErrorActionFactory;
        FactoryPtr<typename FactoryType::ProducedObjectType> errorActionFactory;
        errorActionFactory.as<FactoryType>()->registerCreator("ignore", StandardFileSorterErrorHandlers::ignore);
        errorActionFactory.as<FactoryType>()->registerCreator("continue", StandardFileSorterErrorHandlers::logAndContinue);
        errorActionFactory.as<FactoryType>()->registerCreator("stop", StandardFileSorterErrorHandlers::stop);
        return errorActionFactory;
    }

    auto makeFileExistsFactory() {
        using FactoryType =FileSortingStrategyFactories::CreateValidTargetPathStrategyFactory ;
                FactoryPtr<typename FactoryType::ProducedObjectType>  fileExistsFactory;
        fileExistsFactory.as<FactoryType>()->registerCreator("overwrite", StandardCreateValidTargetPathStrategies::overwrite);
        fileExistsFactory.as<FactoryType>()->registerCreator("skip", StandardCreateValidTargetPathStrategies::skip);
        fileExistsFactory.as<FactoryType>()->registerCreator("abort", StandardCreateValidTargetPathStrategies::abort);
        //@todo maybe rename should be renamed to "addSuffix", because that's what it does. rename could be added as search-and-replace regex
        fileExistsFactory.as<FactoryType>()->registerCreator("rename", StandardCreateValidTargetPathStrategies::rename);
        return fileExistsFactory;
    }
}
#endif// BSC_FACTORIES_H
