//
// Created by Krzysztof Tulidowicz on 09.04.2020.
//

#ifndef BSC_MIMEFILETYPEDECODER_H
#define BSC_MIMEFILETYPEDECODER_H

#include "MimeFileType.h"
#include <filesystem>

namespace bsc {
    namespace fs = std::filesystem;

    class MimeFileTypeDecoder {

    public:
        [[nodiscard]] MimeFileType getTypeForFile(const fs::path& path) const;
    };
}// namespace bsc

#endif