//
// Created by stilgar on 08.09.17.
//

#ifndef BSC_CEREAL_INCLUDE_H
#define BSC_CEREAL_INCLUDE_H


#include <cereal/cereal.hpp>
#include <cereal/access.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/chrono.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/map.hpp>

//@todo remove if cereal updated with C++17:

#ifndef CEREAL_TYPES_STD_OPTIONAL
#define CEREAL_TYPES_STD_OPTIONAL

#include "cereal/cereal.hpp"
#include <optional>
#include <filesystem>

namespace cereal {
    //! Saving for std::optional
    template<class Archive, typename T>
    inline
    void CEREAL_SAVE_FUNCTION_NAME(Archive& ar, const std::optional<T>& optional) {
        if (!optional) {
            ar(CEREAL_NVP_("nullopt", true));
        } else {
            ar(CEREAL_NVP_("nullopt", false),
               CEREAL_NVP_("data", *optional));
        }
    }

    //! Loading for std::optional
    template<class Archive, typename T>
    inline
    void CEREAL_LOAD_FUNCTION_NAME(Archive& ar, std::optional<T>& optional) {
        bool nullopt;
        ar(CEREAL_NVP_("nullopt", nullopt));

        if (nullopt) {
            optional = std::nullopt;
        } else {
            T value;
            ar(CEREAL_NVP_("data", value));
            optional = std::move(value);
        }
    }

    //! Saving for std::filesystem::path
    template<class Archive>
    inline
    void CEREAL_SAVE_FUNCTION_NAME(Archive& ar, const std::filesystem::path& path) {
        ar(CEREAL_NVP_("path", path.string()));
    }

    //! Loading for std::filesystem::path
    template<class Archive>
    inline
    void CEREAL_LOAD_FUNCTION_NAME(Archive& ar, std::filesystem::path& path) {
        std::string pathAsString;
        ar(CEREAL_NVP_("path", pathAsString));
        std::filesystem::path newPath(pathAsString);
        path = std::move(newPath);
    }
} // namespace cereal

#endif // CEREAL_TYPES_STD_OPTIONAL

#endif //BSC_CEREAL_INCLUDE_H
