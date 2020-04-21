//
// Created by stilgar on 20.04.2020.
//

#ifndef BSC_FILELISTFETCHER_H
#define BSC_FILELISTFETCHER_H
#include <filesystem>
#include <vector>
namespace bsc {
    namespace fs = std::filesystem;
    class FileListFetcher {
    public:
        virtual std::vector<fs::path> listFiles(const fs::path& path) = 0;
    };
}// namespace bsc

#endif