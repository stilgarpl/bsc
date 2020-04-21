//
// Created by stilgar on 09.04.2020.
//

#include "MimeFileType.h"
#include <regex>

namespace bsc {

    MimeFileType::MimeFileType(const std::string& typeGroup, const std::string& type)
        : typeGroup(typeGroup), type(type) {}

    MimeFileType MimeFileType::make(std::string mimeString) {
        static const std::regex mimeRegex("(\\w+)/([\\w-]*).*");
        std::smatch mimeMatch;
        if (std::regex_match(mimeString, mimeMatch, mimeRegex)) {
            return MimeFileType(mimeMatch[1], mimeMatch[2]);
        } else {
            throw FileTypeParseException("Mime string [" + mimeString + "] does not match (\\w+)/(\\w+) pattern");
        }
    }

    MimeFileType::MimeFileType(const MimeFileType& other) : typeGroup(other.typeGroup), type(other.type) {}

    MimeFileType::MimeFileType(MimeFileType&& other) : typeGroup(other.typeGroup), type(other.type) {}

    FileTypeParseException::FileTypeParseException(const std::string &arg) : domain_error(arg) {}
}// namespace bsc