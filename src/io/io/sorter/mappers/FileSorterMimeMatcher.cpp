//
// Created by Krzysztof Tulidowicz on 20.04.2020.
//

#include "FileSorterMimeMatcher.h"

#include <utility>

namespace bsc {

    namespace matchers {
        FileSorterMapperMatcher fileSorterMimeMatcher(const MimeFileType& mimeFileType) {
            MimeFileType expectedMimeType(mimeFileType);
            return [expectedMimeType](const FileInfo& fileInfo) {
                if (expectedMimeType.typeGroup == fileInfo.mimeFileType.typeGroup) {
                    if (!expectedMimeType.type.empty()) {
                        if (expectedMimeType.type == fileInfo.mimeFileType.type) {
                            return MatchPrecision::perfect;
                        }

                    } else {
                        return MatchPrecision::partial;
                    }
                }
                return MatchPrecision::none;
            };
        }
    };// namespace matchers

}// namespace bsc