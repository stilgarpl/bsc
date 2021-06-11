//
// Created by Krzysztof Tulidowicz on 20.04.2020.
//
#ifndef BSC_FILELISTFETCHER_H
#define BSC_FILELISTFETCHER_H
#include <filesystem>
#include <functional>
#include <vector>
namespace bsc {
    namespace fs = std::filesystem;

    struct FetcherConfig {
        bool recursive;
    };

    class FileListFetcher final {
    public:
        using Constraint = std::function<bool(const fs::path&)>;
        [[nodiscard]] std::vector<fs::path> listFiles(const fs::path& rootPath) const;
        using FileFetcherFunc = std::function<std::vector<fs::path>(const FetcherConfig&,const fs::path&)>;
    private:
        const FileFetcherFunc fileFetcherFunc;
        const FetcherConfig config;
        const std::vector<Constraint> constraints;
    public:
        FileListFetcher(FileFetcherFunc  fileFetcherFunc, const FetcherConfig& config, std::vector<Constraint>  constraints);
    };
}// namespace bsc

#endif