//
// Created by Krzysztof Tulidowicz on 20.04.2020.
//

#include "FileSorter.h"
#include <core/log/Logger.h>
#include <fmt/format.h>
#include <io/file/FileMetaDataReader.h>
#include <io/file/MimeFileTypeDecoder.h>
#include <io/translator/PathTranslator.h>
#include <iostream>
namespace bsc {
    SortResult FileSorter::sort(const fs::path& pathToSort) {
        SortResult sortResult;
        using namespace std::string_literals;
        try {

            PathTranslator translator;
            MimeFileTypeDecoder decoder;
            //@todo get global properties
            auto fileList = fileListFetcher->listFiles(pathToSort);
            std::ranges::sort(fileList);
            for (const auto& file : fileList) {
                //@todo this could probably be done in parallel
                //@todo first all patterns should be processed, then all files should be copied/moved - what if error
                //@TODO BEST algo - first, scan the file and get file descriptor - with mime, file name and so on. pass
                // descriptor to mapper and ALSO to meta data reader, so mime is not detected twice.
                // happens during processing? half of the files will be processed and half not
                const auto& pattern = mapper.map(file);
                if (pattern.has_value()) {
                    //@todo mapper scans for mime and this does it the second time... maybe add a cache?
                    FileMetaDataReader reader(decoder.getTypeForFile(file));
                    const auto& properties = reader.readMetaData(file);
                    try {
                        auto targetPath = translator.translate(*pattern, properties) / file.filename();
                        LOGGER("target path is " + targetPath.string());
                        if (actions.fileExistsPredicate(targetPath)) {
                            LOGGER("File "s + file.string() + "already exists in location " + targetPath.string())
                            targetPath = actions.createValidTargetPathStrategy(targetPath, actions.fileExistsPredicate);
                            LOGGER("New target path is "s + targetPath.string());
                        }
                        //@todo this if is mostly for skip action to work, but maybe I should throw on invalid target
                        // paths created?
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

                } else {
                    //@todo no mapper handler?
                }
            }
        } catch (const FileSortingException& e) {
            //@todo print error message?
            ERROR("File sorting exception: "s + e.what());
        }
        return sortResult;
    }
    FileSorter::FileSorter(std::unique_ptr<FileListFetcher> fileListFetcher, SortingStrategies actions)
        : actions(std::move(actions)), fileListFetcher(std::move(fileListFetcher)) {}

}// namespace bsc