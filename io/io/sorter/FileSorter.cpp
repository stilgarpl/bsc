//
// Created by stilgar on 20.04.2020.
//

#include "FileSorter.h"
#include <fmt/format.h>
#include <io/translator/PathTranslator.h>
#include <iostream>
namespace bsc {
    FileSorter::SortResult FileSorter::sort(const fs::path& pathToSort) {
        SortResult sortResult;
        try {

            PathTranslator translator;
            MimeFileTypeDecoder decoder;
            //@todo get global properties
            const auto& fileList = fileListFetcher->listFiles(pathToSort);
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
                        //paths created?
                        if (!targetPath.empty()) {

                            actions.sortStrategy(file, targetPath);
                            sortResult.filesMap[file] = targetPath;
                        }
                    } catch (const PathTranslationException& e) {
                        //@todo maybe better error handling?
                        FileSortingException exception("Path transforming failed: "s + e.what());
                        actions.errorHandlerStrategy(file, exception);
                    } catch (const FileSortingException& e) {
                        actions.errorHandlerStrategy(file, e);
                    }

                } else {
                    //@todo no mapper handler?
                }
            }
        } catch (const FileSortingException& e) {
            //@todo print error message?
        }
        return sortResult;
    }
    FileSorter::FileSorter(std::unique_ptr<FileListFetcher> fileListFetcher, SortingStrategies actions)
        : actions(std::move(actions)), fileListFetcher(std::move(fileListFetcher)) {}

    FileSorter::ResultConsumer FileSorter::printResult = [](const FileSorter::SortResult& sortResult) {
        std::cout << "\n Sorted files:\n";
        for (const auto& [from, to] : sortResult.getFilesSortedMap()) {
            std::cout << fmt::format("[{}] => [{}]\n", from.string(), to.string());
        }
        std::cout << std::endl;
    };
}// namespace bsc