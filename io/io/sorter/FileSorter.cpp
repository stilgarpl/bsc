//
// Created by stilgar on 20.04.2020.
//

#include "FileSorter.h"
namespace bsc {
    void FileSorter::sort(const fs::path& pathToSort) {

        const auto& fileList = fileListFetcher->listFiles(pathToSort);
        for (const auto& file : fileList) {
            //@todo this could probably be done in parallel
            //@todo first all patterns should be processed, then all files should be copied/moved - what if error
            //happens during processing? half of the files will be processed and half not
            const auto& pattern = mapper.map(file);
            if (pattern.has_value()) {
                //                PropertiesMetaData const auto& targetPath = parsePattern(pattern);
                //                sortAction->sort(file,targetPath);
            } else {
                //@todo no mapper handler?
            }
        }
    }
    FileSorter::FileSorter(std::unique_ptr<FileListFetcher> fileListFetcher, std::unique_ptr<SortAction> sortAction)
        : sortAction(std::move(sortAction)), fileListFetcher(std::move(fileListFetcher)) {}

}// namespace bsc