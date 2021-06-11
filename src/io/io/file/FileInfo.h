//
// Created by stilgar on 11.06.2021.
//

#ifndef BSC_FILEINFO_H
#define BSC_FILEINFO_H

#include "MimeFileType.h"
namespace bsc {
    struct FileInfo {
        const MimeFileType mimeFileType;
        const fs::path path;
    };
}
#endif// BSC_FILEINFO_H
