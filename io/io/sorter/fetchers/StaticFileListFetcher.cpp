//
// Created by stilgar on 20.04.2020.
//

#include "StaticFileListFetcher.h"

#include <utility>
namespace bsc {
    std::vector<fs::path> bsc::StaticFileListFetcher::doListFiles(const fs::path& path) { return fileList; }
    bsc::StaticFileListFetcher::StaticFileListFetcher(std::vector<fs::path> fileList) : fileList(std::move(fileList)) {}
}// namespace bsc
