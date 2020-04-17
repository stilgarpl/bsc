//
// Created by stilgar on 11.04.2020.
//

#ifndef BSC_FILEMETADATAREADER_H
#define BSC_FILEMETADATAREADER_H

#include "FileMetaData.h"
#include "reader/FileReaderAlgorithm.h"
#include <filesystem>
#include <vector>

namespace bsc {
    namespace fs = std::filesystem;
    class FileMetaDataReader {
    private:
        FileType fileType;

    public:
        FileMetaDataReader(FileType type);

        FileMetaData readMetaData(const fs::path&);
    };

}// namespace bsc

#endif