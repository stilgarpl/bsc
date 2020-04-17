//
// Created by stilgar on 11.04.2020.
//

#ifndef BSC_FILEMETADATA_H
#define BSC_FILEMETADATA_H

namespace bsc {
    class FileMetaData {
        std::map<std::string, std::string> metadata;

    public:
        std::string& operator[](const std::string& key) { return metadata[key]; }

        std::optional<std::string_view> operator[](const std::string& key) const {
            try {
                return metadata.at(key);
            } catch (...) {
                //@todo handle exception better
                return std::nullopt;
            }
        }

        FileMetaData& operator+=(const FileMetaData& other) {
            for (const auto& [key, value] : other.metadata) {
                // this overwrites values for keys that are already present. this is by design, so specific readers can
                // overwrite values for generic reader that were run before.
                metadata[key] = value;
            }
            return *this;
        }
    };
}// namespace bsc

#endif