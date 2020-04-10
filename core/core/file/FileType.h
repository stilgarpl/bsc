//
// Created by stilgar on 09.04.2020.
//

#ifndef BSC_FILETYPE_H
#define BSC_FILETYPE_H

#include <string>

namespace bsc {
    struct FileType {
        const std::string typeGroup;
        const std::string type;
    private:
        FileType(const std::string &typeGroup, const std::string &type);

        FileType(const FileType &other);

        FileType(const FileType &&other);

    public:
        static FileType make(std::string mimeString);

    };


    class FileTypeParseException : public std::domain_error {
    public:
        FileTypeParseException(const std::string &arg);
    };
}// namespace bsc

#endif