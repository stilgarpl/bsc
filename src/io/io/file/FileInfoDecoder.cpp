//
// Created by stilgar on 11.06.2021.
//

#include "FileInfoDecoder.h"
bsc::FileInfo bsc::FileInfoDecoder::decodeFileInfo(const fs::path& path) const {
    return FileInfo{.mimeFileType = mimeFileTypeDecoder.getTypeForFile(path), .path = path};
}