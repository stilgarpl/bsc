//
// Created by Krzysztof Tulidowicz on 20.04.2020.
//

#ifndef BSC_STATICFILELISTFETCHER_H
#define BSC_STATICFILELISTFETCHER_H

#include "FileListFetcher.h"
namespace bsc {
    class StaticFileListFetcher {
    private:
        const std::vector<fs::path> fileList;

    public:
        explicit StaticFileListFetcher(std::vector<fs::path> fileList);

    public:
        std::vector<fs::path> operator()(const FetcherConfig& config, const fs::path& path);
    };

}// namespace bsc

#endif