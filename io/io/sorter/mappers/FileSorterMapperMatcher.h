//
// Created by stilgar on 20.04.2020.
//

#ifndef BSC_FILESORTERMAPPERMATCHER_H
#define BSC_FILESORTERMAPPERMATCHER_H
#include <filesystem>
namespace bsc {
    namespace fs = std::filesystem;

    enum class MatchPrecision {
        none    = 0,
        partial = 16,
        perfect = 64,
    };

    //@todo convert to std::function like SortAction
    class FileSorterMapperMatcher {
    public:
        virtual MatchPrecision matches(const fs::path& path) = 0;
    };
}// namespace bsc

#endif