//
// Created by Krzysztof Tulidowicz on 20.04.2020.
//

#ifndef BSC_FILESYSTEMFILELISTFETCHER_H
#define BSC_FILESYSTEMFILELISTFETCHER_H

#include "FileListFetcher.h"
namespace bsc {
    /**
     * lists all files that are in supplied path
     */
    class FilesystemFileListFetcher {
    public:
        std::vector<fs::path> operator()(const FetcherConfig& config, const fs::path& path) const;
    };

    namespace fetchers {
        inline const FilesystemFileListFetcher filesystemFileListFetcher{};
    }
}// namespace bsc

#endif