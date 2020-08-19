//
// Created by Krzysztof Tulidowicz on 20.04.2020.
//

#ifndef BSC_STATICFILELISTFETCHER_H
#define BSC_STATICFILELISTFETCHER_H

#include "FileListFetcher.h"
namespace bsc {
    class StaticFileListFetcher : public FileListFetcher {
    private:
        const std::vector<fs::path> fileList;

    public:
        StaticFileListFetcher(std::vector<fs::path> fileList);

    public:
        std::vector<fs::path> doListFiles(const fs::path& path) override;
    };
}// namespace bsc

#endif