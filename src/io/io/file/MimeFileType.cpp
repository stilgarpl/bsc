//
// Created by Krzysztof Tulidowicz on 09.04.2020.
//

#include "MimeFileType.h"
#include <regex>
#include <utility>

namespace bsc {

    MimeFileType::MimeFileType(std::string typeGroup, std::string type) : typeGroup(std::move(typeGroup)), type(std::move(type)) {}

    FileTypeParseException::FileTypeParseException(const std::string& arg) : domain_error(arg) {}
    MimeFileType MimeFileTypeFactory::create(const std::string& mimeString) const {
        //@todo make / optional, so you can have "image" but required for "image/jpeg"
        static const std::regex mimeRegex("(\\w+)/([\\w-]*).*");
        std::smatch mimeMatch;
        if (std::regex_match(mimeString, mimeMatch, mimeRegex)) {
            return MimeFileType(mimeMatch[1], mimeMatch[2]);
        } else {
            throw FileTypeParseException("Mime string [" + mimeString + "] does not match (\\w+)/(\\w+) pattern");
        }
    }
}// namespace bsc