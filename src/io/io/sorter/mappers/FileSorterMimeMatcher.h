//
// Created by Krzysztof Tulidowicz on 20.04.2020.
//

#ifndef BSC_FILESORTERMIMEMATCHER_H
#define BSC_FILESORTERMIMEMATCHER_H

#include "FileSorterMapperMatcher.h"
#include <io/file/MimeFileType.h>
#include <io/file/MimeFileTypeDecoder.h>
namespace bsc {
    class FileSorterMimeMatcher final : public FileSorterMapperMatcher {
    private:
        MimeFileType expectedMimeType;
        MimeFileTypeDecoder decoder;

    public:
        explicit FileSorterMimeMatcher(MimeFileType mimeFileType);
        MatchPrecision matches(const fs::path& path) override;
        ~FileSorterMimeMatcher() override = default;
    };
}// namespace bsc

#endif