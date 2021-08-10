//
// Created by Krzysztof Tulidowicz on 20.04.2020.
//

#include "FileSorter.h"
#include <fmt/format.h>
#include <io/file/FileMetaDataReader.h>
#include <iostream>
#include <log/log/Logger.h>
#include <range/v3/to_container.hpp>
#include <ranges>
namespace bsc {
    SortResult FileSorter::sort(const std::vector<fs::path>& pathsToSort) {
        using namespace std::string_literals;
        using namespace std::ranges::views;
        SortResult sortResult;

        auto decodeFileInfo       = [this](const auto& file) { return decoder.decodeFileInfo(file); };
        auto addMapperToFileInfo  = [this](const auto& fileInfo) { return std::make_tuple(fileInfo, mapper.map(fileInfo)); };
        auto filterMappedPatterns = [](const auto& fileInfoWithPattern) { return std::get<1>(fileInfoWithPattern).has_value(); };
        auto addProperties        = [](const auto& fileInfoWithPattern) {
            struct FileInfoWithPatternAndProperties {
                FileInfo fileInfo;
                std::string pattern;
                PropertiesMetaData propertiesMetaData;
            };
            const auto& fileInfo = std::get<0>(fileInfoWithPattern);
            FileMetaDataReader reader(fileInfo.mimeFileType);
            return FileInfoWithPatternAndProperties{.fileInfo           = std::get<0>(fileInfoWithPattern),
                                                    .pattern            = *std::get<1>(fileInfoWithPattern),
                                                    .propertiesMetaData = reader.readMetaData(fileInfo.path)};
        };

        try {
            auto fileList = fetchAllFiles(pathsToSort);

            auto filePatternAndPropertiesList = fileList
                                                | transform(decodeFileInfo)
                                                | transform(addMapperToFileInfo)
                                                | filter(filterMappedPatterns)
                                                | transform(addProperties);

            for (const auto& filePatternAndProperties : filePatternAndPropertiesList) {
                const auto& file = filePatternAndProperties.fileInfo.path;
                try {
                    //@todo add global properties, like current time or computer name etc.
                    auto targetPath = translator.translate(filePatternAndProperties.pattern, filePatternAndProperties.propertiesMetaData) /
                                      file.filename();
                    logger.debug("target path is " + targetPath.string());
                    if (actions.fileExistsPredicate(targetPath)) {
                        logger.debug("File "s + file.string() + "already exists in location " + targetPath.string());
                        targetPath = actions.createValidTargetPathStrategy(targetPath, actions.fileExistsPredicate);
                        logger.debug("New target path is "s + targetPath.string());
                    }
                    //@todo this if is mostly for skip action to work, but maybe I should throw on invalid target
                    // paths created?
                    //@todo target path should not be a simple path but perhaps wrapper object with path and result? sort of like Either or expected ?
                    if (!targetPath.empty()) {
                        actions.sortStrategy(file, targetPath);
                        sortResult.sortedFilesMap[file] = targetPath;
                    }
                } catch (const PathTranslationException& e) {
                    //@todo maybe better error handling?
                    FileSortingException exception("Path transforming failed: "s + e.what(), file);
                    actions.errorHandlerStrategy(file, exception, sortResult.failedFilesList);
                } catch (const FileSortingException& e) {
                    FileSortingException exception(e.what(), file, e.getDestinationPath());
                    actions.errorHandlerStrategy(file, exception, sortResult.failedFilesList);
                }
            }
        } catch (const FileSortingException& e) {
            //@todo print error message?
            logger.error("File sorting exception: "s + e.what());
        }
        return sortResult;
    }
    std::vector<fs::path> FileSorter::fetchAllFiles(const std::vector<fs::path>& pathsToSort) const {
        std::vector<fs::path> fileList;
        for (const auto& path : pathsToSort) {
            auto list = fileListFetcher.listFiles(path);
            fileList.reserve(fileList.size() + list.size());
            std::ranges::copy(list, std::back_inserter(fileList));
        }
        std::ranges::sort(fileList);
        return fileList;
    }
    FileSorter::FileSorter(FileListFetcher fileListFetcher, SortingStrategies actions)
        : actions(std::move(actions)), fileListFetcher(std::move(fileListFetcher)) {
    }

}// namespace bsc