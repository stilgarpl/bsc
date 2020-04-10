//
// Created by stilgar on 09.04.2020.
//

#include "FileType.h"
#include <regex>

namespace bsc {

    FileType::FileType(const std::string &typeGroup, const std::string &type) : typeGroup(typeGroup), type(type) {}

    FileType FileType::make(std::string mimeString) {
        static const std::regex mimeRegex("(\\w+)/([\\w-]+).*");
        std::smatch mimeMatch;
        if (std::regex_match(mimeString, mimeMatch, mimeRegex)) {
            return FileType(mimeMatch[1], mimeMatch[2]);
        } else {
            throw FileTypeParseException("Mime string [" + mimeString + "] does not match (\\w+)/(\\w+) pattern");
        }
    }

    FileType::FileType(const FileType &other) : typeGroup(other.typeGroup), type(other.type) {}

    FileType::FileType(const FileType &&other) : typeGroup(std::move(other.typeGroup)), type(std::move(other.type)) {}

    FileTypeParseException::FileTypeParseException(const std::string &arg) : domain_error(arg) {}
}// namespace bsc