//
// Created by stilgar on 20.04.2020.
//

#include "FileSorter.h"
#include <io/translator/PathTranslator.h>
namespace bsc {
    void FileSorter::sort(const fs::path& pathToSort) {
        PathTranslator translator;
        MimeFileTypeDecoder decoder;
        //@todo get global properties
        const auto& fileList = fileListFetcher->listFiles(pathToSort);
        for (const auto& file : fileList) {
            //@todo this could probably be done in parallel
            //@todo first all patterns should be processed, then all files should be copied/moved - what if error
            //@TODO BEST algo - first, scan the file and get file descriptor - with mime, file name and so on. pass
            //descriptor to mapper and ALSO to meta data reader, so mime is not detected twice.
            // happens during processing? half of the files will be processed and half not
            const auto& pattern = mapper.map(file);
            if (pattern.has_value()) {
                //@todo mapper scans for mime and this does it the second time... maybe add a cache?
                FileMetaDataReader reader(decoder.getTypeForFile(file));
                const auto& properties = reader.readMetaData(file);
                try {
                    const auto& targetPath = translator.translate(*pattern, properties);
                    sortAction(file, targetPath / file.filename());
                } catch (const std::exception& e) {
                    ERROR("Exception : "s + e.what());
                }

            } else {
                //@todo no mapper handler?
            }
        }
    }
    FileSorter::FileSorter(std::unique_ptr<FileListFetcher> fileListFetcher, SortAction sortAction)
        : sortAction(std::move(sortAction)), fileListFetcher(std::move(fileListFetcher)) {}

}// namespace bsc