//
// Created by Krzysztof Tulidowicz on 08.05.2020.
//

#include "FileListFetcher.h"
#include <numeric>
#include <range/v3/to_container.hpp>
#include <ranges>
#include <utility>
namespace bsc {
    std::vector<fs::path> FileListFetcher::listFiles(const std::filesystem::path& rootPath) const {
        namespace views = std::ranges::views;
        auto files      = fileFetcherFunc(config, rootPath);
        std::vector<fs::path> result =
                files | views::filter([this](const auto& path) {
                    //@todo C++23 use std::ranges::fold when available
                    return std::accumulate(constraints.begin(), constraints.end(), true, [&path](bool result, const auto& constraint) {
                        // short-circuit
                        if (result) {
                            return constraint(path);
                        } else {
                            return result;
                        }
                    });
                }) |
                ranges::to<std::vector<fs::path>>();
        return result;
    }
    FileListFetcher::FileListFetcher(FileListFetcher::FileFetcherFunc  fileFetcherFunc,
                                     const FetcherConfig& config,
                                     std::vector<Constraint>  constraints)
        : fileFetcherFunc(std::move(fileFetcherFunc)), config(config), constraints(std::move(constraints)) {
    }


}// namespace bsc