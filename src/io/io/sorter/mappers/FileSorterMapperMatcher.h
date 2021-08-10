//
// Created by Krzysztof Tulidowicz on 20.04.2020.
//

#ifndef BSC_FILESORTERMAPPERMATCHER_H
#define BSC_FILESORTERMAPPERMATCHER_H
#include <filesystem>
#include <functional>
#include <io/file/FileInfo.h>
namespace bsc {
    namespace fs = std::filesystem;

    enum class MatchPrecision {
        none    = 0,
        partial = 16,
        perfect = 64,
    };


    using FileSorterMapperMatcher = std::function<MatchPrecision(const FileInfo& path)>;
}// namespace bsc

#endif