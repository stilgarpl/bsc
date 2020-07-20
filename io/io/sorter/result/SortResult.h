//
// Created by Krzysztof Tulidowicz on 16.06.2020.
//

#ifndef BSC_SORTRESULT_H
#define BSC_SORTRESULT_H

#include <filesystem>
#include <functional>
#include <list>
#include <map>
namespace bsc {
    namespace fs = std::filesystem;
    class SortResult;
    using ResultConsumer = std::function<void(const SortResult&)>;
    struct SortFailure {
        fs::path sourcePath{};
        std::optional<fs::path> destinationPath{};
        std::string errorMessage{};
    };
    class SortResult {
        std::map<fs::path, fs::path> sortedFilesMap;
        std::list<SortFailure> failedFilesList;
        friend class FileSorter;

    public:
        [[nodiscard]] const decltype(sortedFilesMap)& getSortedFiles() const { return sortedFilesMap; }
        [[nodiscard]] const decltype(failedFilesList)& getFailedFiles() const { return failedFilesList; }
        const SortResult& operator|(const ResultConsumer& consumer) const {
            consumer(*this);
            return *this;
        }
    };

    struct StandardResultConsumers {
        static ResultConsumer printResult;
    };
}// namespace bsc

#endif