//
// Created by Krzysztof Tulidowicz on 09.12.2021.
//

#include "Strategies.h"
#include "StandardFileSorterStrategies.h"
bsc::FileSortingStrategies::FileSortingStrategies()
    : sortStrategy(StandardFileSorterSortStrategies::copy),
      createValidTargetPathStrategy(StandardCreateValidTargetPathStrategies::rename()),
      errorHandlerStrategy(StandardFileSorterErrorHandlers::stop) {
}
bsc::FileSortingStrategies::FileSortingStrategies(
        const bsc::FileSortingStrategies::SortStrategy& sortStrategy,
        const bsc::FileSortingStrategies::CreateValidTargetPathStrategy& createValidTargetPathStrategy,
        const bsc::FileSortingStrategies::ErrorHandlingStrategy& errorHandlerStrategy)
    : sortStrategy(sortStrategy), createValidTargetPathStrategy(createValidTargetPathStrategy), errorHandlerStrategy(errorHandlerStrategy) {
}
