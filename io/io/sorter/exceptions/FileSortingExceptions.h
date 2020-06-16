//
// Created by stilgar on 08.06.2020.
//

#ifndef BSC_FILESORTINGEXCEPTIONS_H
#define BSC_FILESORTINGEXCEPTIONS_H

#include <stdexcept>
namespace bsc {
    class FileSortingException : public std::domain_error {
        fs::path sourcePath{};
        std::optional<fs::path> destinationPath{};

    public:
        FileSortingException(const std::string& message,
                             const fs::path& sourcePath,
                             const std::optional<fs::path>& destinationPath);
        FileSortingException(const std::string& errorMessage, const fs::path& sourcePath);
        [[nodiscard]] const fs::path& getSourcePath() const;
        [[nodiscard]] const std::optional<fs::path>& getDestinationPath() const;
    };
}// namespace bsc

#endif