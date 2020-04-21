//
// Created by stilgar on 20.04.2020.
//

#ifndef BSC_FILESORTERMAPPER_H
#define BSC_FILESORTERMAPPER_H
#include "FileSorterMapperMatcher.h"
#include <filesystem>
#include <optional>
namespace bsc {
    namespace fs = std::filesystem;
    /**
     * mapper that matches something about the file (mime type, filename) to destination path where it should be
     */
    class FileSorterMapper {
        //@todo "this should have vector of pair<matcher,path-pattern> and a pattern parser - also, something that fills
        //properties. some properties could be provided in constructor, and other read from file"
        std::vector<std::pair<std::unique_ptr<FileSorterMapperMatcher>, std::string>> patterns;

    public:
        std::optional<std::string> map(const fs::path& from);
        void addPattern(std::unique_ptr<FileSorterMapperMatcher> matcher, std::string pattern) {
            patterns.push_back(
                    std::make_pair<std::unique_ptr<FileSorterMapperMatcher>, std::string>(std::move(matcher),
                                                                                          std::move(pattern)));
        }
    };

}// namespace bsc

#endif