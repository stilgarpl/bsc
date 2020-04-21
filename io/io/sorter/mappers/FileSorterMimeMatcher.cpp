//
// Created by stilgar on 20.04.2020.
//

#include "FileSorterMimeMatcher.h"

namespace bsc {

    FileSorterMimeMatcher::FileSorterMimeMatcher(const MimeFileType& mimeFileType) : expectedMimeType(mimeFileType) {}
    MatchPrecision FileSorterMimeMatcher::matches(const fs::path& path) {
        if (fs::exists(path)) {
            const auto& decodedMime = decoder.getTypeForFile(path);
            if (expectedMimeType.typeGroup == decodedMime.typeGroup) {
                if (!expectedMimeType.type.empty()) {
                    if (expectedMimeType.type == decodedMime.type) {
                        return MatchPrecision::perfect;
                    }

                } else {
                    return MatchPrecision::partial;
                }
            }
        }
        return MatchPrecision::none;
    }
}// namespace bsc