//
// Created by stilgar on 08.06.2020.
//

#include "FileSortingExceptions.h"

namespace bsc {

    FileSortingException::FileSortingException(const std::string& errorMessage, const fs::path& sourcePath)
        : domain_error(errorMessage), sourcePath(sourcePath) {}
    const fs::path& FileSortingException::getSourcePath() const { return sourcePath; }
    const std::optional<fs::path>& FileSortingException::getDestinationPath() const { return destinationPath; }
    FileSortingException::FileSortingException(const std::string& message,
                                               const fs::path& sourcePath,
                                               const std::optional<fs::path>& destinationPath)
        : domain_error(message), sourcePath(sourcePath), destinationPath(destinationPath) {}
}// namespace bsc
