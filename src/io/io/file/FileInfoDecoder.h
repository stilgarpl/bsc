//
// Created by stilgar on 11.06.2021.
//

#ifndef BSC_FILEINFODECODER_H
#define BSC_FILEINFODECODER_H

#include "FileInfo.h"
#include "MimeFileTypeDecoder.h"
namespace bsc {
    class FileInfoDecoder {
    private:
        MimeFileTypeDecoder mimeFileTypeDecoder{};
    public:

        FileInfo decodeFileInfo(const fs::path& path) const;
    };
}

#endif// BSC_FILEINFODECODER_H
