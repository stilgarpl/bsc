//
// Created by Krzysztof Tulidowicz on 08.09.17.
//

#ifndef BSC_CEREAL_INCLUDE_H
#define BSC_CEREAL_INCLUDE_H

//@todo there is a bug in 1.3.0, this is a workaround. remove when upgraded to 1.3.1
#define CEREAL_THREAD_SAFE 1

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
#include <cereal/types/optional.hpp>
#include <cereal/types/variant.hpp>


#include "cereal/cereal.hpp"
#include <filesystem>

namespace cereal {

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




#endif //BSC_CEREAL_INCLUDE_H
