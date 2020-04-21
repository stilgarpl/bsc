//
// Created by stilgar on 11.04.2020.
//

#ifndef BSC_PROPERTIESMETADATA_H
#define BSC_PROPERTIESMETADATA_H
#include <map>
#include <nlohmann/json.hpp>
namespace bsc {
    using PropertiesMetaData = nlohmann::json;
    //    class PropertiesMetaData {
    //        std::map<std::string, std::string> metadata;
    //
    //    public:
    //        std::string& operator[](const std::string& key) { return metadata[key]; }
    //
    //        [[nodiscard]] auto begin() const -> decltype(metadata.begin()) {
    //            return metadata.begin();
    //        }
    //
    //        [[nodiscard]] auto end() const -> decltype(metadata.end()) {
    //            return metadata.end();
    //        }
    //
    //
    //        std::optional<std::string_view> operator[](const std::string& key) const {
    //            if(metadata.contains(key)) {
    //                return metadata.at(key);
    //            } else {
    //                return std::nullopt;
    //            }
    //        }
    //
    //        PropertiesMetaData& operator+=(const PropertiesMetaData& other) {
    //            for (const auto& [key, value] : other.metadata) {
    //                // this overwrites values for keys that are already present. this is by design, so specific
    //                readers can
    //                // overwrite values for generic reader that were run before.
    //                metadata[key] = value;
    //            }
    //            return *this;
    //        }
    //    };
}// namespace bsc

#endif