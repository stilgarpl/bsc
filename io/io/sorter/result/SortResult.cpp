//
// Created by stilgar on 16.06.2020.
//

#include "SortResult.h"
#include <fmt/format.h>
#include <iostream>

namespace bsc {
    ResultConsumer StandardResultConsumers::printResult = [](const SortResult& sortResult) {
        using namespace std::string_literals;
        std::cout << "\n Sorted files:\n";
        for (const auto& [from, to] : sortResult.getSortedFiles()) {
            std::cout << fmt::format("[{}] => [{}]\n", from.string(), to.string());
        }
        if (!sortResult.getFailedFiles().empty()) {
            std::cout << "\n Failures:\n";
            for (const auto& failedFile : sortResult.getFailedFiles()) {
                std::cout << fmt::format("Sorting file {} ", failedFile.sourcePath.string())
                          << (failedFile.destinationPath
                                      ? fmt::format("to {} ", failedFile.destinationPath.value().string())
                                      : ""s)
                          << fmt::format("with error: {}\n", failedFile.errorMessage);
            }
        }
        std::cout << fmt::format("\n Files count: {}\n", sortResult.getSortedFiles().size());
        std::cout << std::endl;
    };

}
