//
// Created by stilgar on 20.04.2020.
//

#ifndef BSC_FILESORTERMIMEMATCHER_H
#define BSC_FILESORTERMIMEMATCHER_H

#include "FileSorterMapperMatcher.h"
#include <io/file/MimeFileType.h>
#include <io/file/MimeFileTypeDecoder.h>
namespace bsc {
    class FileSorterMimeMatcher : public FileSorterMapperMatcher {
    private:
        MimeFileType expectedMimeType;
        MimeFileTypeDecoder decoder;

    public:
        FileSorterMimeMatcher(MimeFileType mimeFileType);
        FileSorterMimeMatcher(const std::string& mimeString);
        MatchPrecision matches(const fs::path& path) override;
    };
}// namespace bsc

#endif