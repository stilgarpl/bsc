//
// Created by Krzysztof Tulidowicz on 20.04.2020.
//

#include "StaticFileListFetcher.h"

namespace bsc {
    std::vector<fs::path> bsc::StaticFileListFetcher::operator()(const FetcherConfig& config,const fs::path& path) { return fileList; }
    bsc::StaticFileListFetcher::StaticFileListFetcher(std::vector<fs::path> fileList) : fileList(std::move(fileList)) {}
}// namespace bsc
