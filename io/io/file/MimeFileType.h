//
// Created by Krzysztof Tulidowicz on 09.04.2020.
//

#ifndef BSC_MIMEFILETYPE_H
#define BSC_MIMEFILETYPE_H

#include <stdexcept>
#include <string>

namespace bsc {
    struct MimeFileType {
        const std::string typeGroup;
        const std::string type;

    private:
        MimeFileType(std::string typeGroup, std::string type);

    public:
        MimeFileType(const MimeFileType& other)     = default;
        MimeFileType(MimeFileType&& other) noexcept = default;
        static MimeFileType make(const std::string& mimeString);
    };


    class FileTypeParseException : public std::domain_error {
    public:
        explicit FileTypeParseException(const std::string& arg);
    };
}// namespace bsc

#endif