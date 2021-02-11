//
// Created by Krzysztof Tulidowicz on 08.05.2020.
//

#include "FileListFetcher.h"
#include <numeric>
#include <range/v3/to_container.hpp>
#include <ranges>
namespace bsc {
    std::vector<fs::path> FileListFetcher::listFiles(const std::filesystem::path& rootPath) {
        namespace views = std::ranges::views;
        auto files      = doListFiles(rootPath);
        //@todo replace with ranges C++20
        //        files.erase(std::remove_if(files.begin(),
        //                                   files.end(),
        //                                   [this](const auto& path) {
        //                                       return std::accumulate(constraints.begin(),
        //                                                              constraints.end(),
        //                                                              false,
        //                                                              [&path](bool result, const auto& constraint) {
        //                                                                  //@todo add test for constraints.
        //                                                                  // short-circuit
        //                                                                  if (!result) {
        //                                                                      return !constraint(path);
        //                                                                  } else {
        //                                                                      return result;
        //                                                                  }
        //                                                              });
        //                                   }),
        //                    files.end());
        //        @todo implement constraints
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
}// namespace bsc