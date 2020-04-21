//
// Created by stilgar on 11.04.2020.
//

#ifndef BSC_FILEMETADATAREADER_H
#define BSC_FILEMETADATAREADER_H

#include <filesystem>
#include <io/file/MimeFileType.h>
#include <io/translator/PropertiesMetaData.h>
#include <vector>

namespace bsc {
    namespace fs = std::filesystem;
    class FileMetaDataReader {
    private:
        MimeFileType fileType;

    public:
        FileMetaDataReader(MimeFileType type);

        PropertiesMetaData readMetaData(const fs::path&);
    };

}// namespace bsc

#endif