//
// Created by stilgar on 20.04.2020.
//
#ifndef BSC_FILELISTFETCHER_H
#define BSC_FILELISTFETCHER_H
#include <filesystem>
#include <functional>
#include <vector>
namespace bsc {
    namespace fs = std::filesystem;
    //@todo maybe file list fetcher should take additional constraints, like modification date range?
    class FileListFetcher {
    public:
        using Constraint = std::function<bool(const fs::path&)>;

    protected:
        //@todo this too can be converted to std::function and stored in this class, instead of inheriting - this class
        //could wrap around fetcher and name should be changed.
        virtual std::vector<fs::path> doListFiles(const fs::path& rootPath) = 0;

    private:
        std::vector<Constraint> constraints;

    public:
        std::vector<fs::path> listFiles(const fs::path& rootPath);
    };
}// namespace bsc

#endif