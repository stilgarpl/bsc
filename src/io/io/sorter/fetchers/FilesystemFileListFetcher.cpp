//
// Created by Krzysztof Tulidowicz on 20.04.2020.
//

#include "FilesystemFileListFetcher.h"
namespace bsc {
    std::vector<fs::path> bsc::FilesystemFileListFetcher::operator()(const FetcherConfig& config, const fs::path& path) {

        if (!fs::exists(path)) {
            return {};
        }
        if (!fs::is_directory(path)) {
            return {path};
        }

        std::vector<fs::path> result;
        if (config.recursive) {
            for (auto& entry : fs::recursive_directory_iterator(path)) {
                if (entry.is_regular_file() && !entry.is_directory()) {
                    result.push_back(entry.path());
                }
            }
        } else {
            for (auto& entry : fs::directory_iterator(path)) {
                if (entry.is_regular_file() && !entry.is_directory()) {
                    result.push_back(entry.path());
                }
            }
        }
        return result;
    }

}// namespace bsc