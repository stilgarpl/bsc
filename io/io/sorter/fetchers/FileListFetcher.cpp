//
// Created by stilgar on 08.05.2020.
//

#include "FileListFetcher.h"
#include <numeric>
namespace bsc {
    std::vector<fs::path> FileListFetcher::listFiles(const std::filesystem::path& rootPath) {
        auto files = doListFiles(rootPath);
        //@todo replace with ranges
        files.erase(std::remove_if(files.begin(),
                                   files.end(),
                                   [this](const auto& path) {
                                       return std::accumulate(constraints.begin(),
                                                              constraints.end(),
                                                              false,
                                                              [&path](bool result, const auto& constraint) {
                                                                  //@todo add test for constraints.
                                                                  // short-circuit
                                                                  if (!result) {
                                                                      return !constraint(path);
                                                                  } else {
                                                                      return result;
                                                                  }
                                                              });
                                   }),
                    files.end());
        //@todo implement constraints
        //        std::vector<fs::path> result =  files | views::filter([this](const auto& path) {
        //            return std::accumulate(constraints.begin(), constraints.end(), true, [&path](bool result, const
        //            auto& constraint) {
        //                // short-circuit
        //                if (!result) {
        //                    return result && constraint(path);
        //                } else {
        //                    return result;
        //                }
        //            });
        //        });
        return files;
    }
}// namespace bsc