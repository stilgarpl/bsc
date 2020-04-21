//
// Created by stilgar on 09.04.2020.
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
        MimeFileType(const std::string& typeGroup, const std::string& type);

    public:
        MimeFileType(const MimeFileType& other);
        MimeFileType(MimeFileType&& other);
        static MimeFileType make(std::string mimeString);
    };


    class FileTypeParseException : public std::domain_error {
    public:
        FileTypeParseException(const std::string &arg);
    };
}// namespace bsc

#endif