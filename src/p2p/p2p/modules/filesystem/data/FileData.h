//
// Created by Krzysztof Tulidowicz on 04.11.18.
//

#ifndef BSC_FILEDATA_H
#define BSC_FILEDATA_H


#include <filesystem>

namespace bsc {
    namespace fs = std::filesystem;

    class FileData {
    private:
        fs::path canonicalPath;
        std::string sha256hash;
        fs::perms permissions;
        uintmax_t size = 0;
        fs::file_time_type modificationTime = fs::file_time_type::min();
        bool isDirectory = false;

    public:
        [[nodiscard]] bool isIsDirectory() const;

    public:
        [[nodiscard]] const std::filesystem::path& getCanonicalPath() const;

        [[nodiscard]] const std::string& getSha256hash() const;

        [[nodiscard]] fs::perms getPermissions() const;

        [[nodiscard]] const uintmax_t& getSize() const;

        [[nodiscard]] const fs::file_time_type& getModificationTime() const;

        explicit FileData(const fs::path& path);

        FileData(std::filesystem::path canonicalPath, std::string sha256hash,
                 std::filesystem::perms permissions, uintmax_t size, fs::file_time_type modificationTime,
                 bool isDirectory);

    };
}


#endif //BSC_FILEDATA_H
