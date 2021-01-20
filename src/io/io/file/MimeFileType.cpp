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
        static const std::regex mimeRegex(R"((\w+)(/[\w-\.]*)?.*)");
        std::smatch mimeMatch;
        if (std::regex_match(mimeString, mimeMatch, mimeRegex)) {
            auto& group = mimeMatch[1];
            auto& type  = mimeMatch[2];
            // remove first character "/" from type string.
            return MimeFileType(group, type.str().empty() ? type : type.str().substr(1));
        } else {
            throw FileTypeParseException("Mime string [" + mimeString + "] does not match (\\w+)/(\\w+) pattern");
        }
    }
}// namespace bsc