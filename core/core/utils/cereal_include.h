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


//@todo remove when cereal updated with variant :
#ifndef CEREAL_TYPES_STD_VARIANT_HPP_
#define CEREAL_TYPES_STD_VARIANT_HPP_

#include "cereal/cereal.hpp"
#include <variant>
#include <cstdint>

namespace cereal
{
    namespace variant_detail
    {
        //! @internal
        template <class Archive>
        struct variant_save_visitor
        {
            variant_save_visitor(Archive & ar_) : ar(ar_) {}

            template<class T>
            void operator()(T const & value) const
            {
                ar( CEREAL_NVP_("data", value) );
            }

            Archive & ar;
        };

        //! @internal
        template<int N, class Variant, class ... Args, class Archive>
        typename std::enable_if<N == std::variant_size_v<Variant>, void>::type
        load_variant(Archive & /*ar*/, int /*target*/, Variant & /*variant*/)
        {
            throw ::cereal::Exception("Error traversing variant during load");
        }
        //! @internal
        template<int N, class Variant, class H, class ... T, class Archive>
        typename std::enable_if<N < std::variant_size_v<Variant>, void>::type
        load_variant(Archive & ar, int target, Variant & variant)
        {
            if(N == target)
            {
                H value;
                ar( CEREAL_NVP_("data", value) );
                variant = std::move(value);
            }
            else
                load_variant<N+1, Variant, T...>(ar, target, variant);
        }

    } // namespace variant_detail

    //! Saving for std::variant
    template <class Archive, typename VariantType1, typename... VariantTypes> inline
    void CEREAL_SAVE_FUNCTION_NAME( Archive & ar, std::variant<VariantType1, VariantTypes...> const & variant )
    {
        std::int32_t index = static_cast<std::int32_t>(variant.index());
        ar( CEREAL_NVP_("index", index) );
        variant_detail::variant_save_visitor<Archive> visitor(ar);
        std::visit(visitor, variant);
    }

    //! Loading for std::variant
    template <class Archive, typename... VariantTypes> inline
    void CEREAL_LOAD_FUNCTION_NAME( Archive & ar, std::variant<VariantTypes...> & variant )
    {
        using variant_t = typename std::variant<VariantTypes...>;

        std::int32_t index;
        ar( CEREAL_NVP_("index", index) );
        if(index >= static_cast<std::int32_t>(std::variant_size_v<variant_t>))
            throw Exception("Invalid 'index' selector when deserializing std::variant");

        variant_detail::load_variant<0, variant_t, VariantTypes...>(ar, index, variant);
    }

    //! Serializing a std::monostate
    template <class Archive>
    void CEREAL_SERIALIZE_FUNCTION_NAME( Archive &, std::monostate const & ) {}
} // namespace cereal

#endif // CEREAL_TYPES_STD_VARIANT_HPP_

#endif // CEREAL_TYPES_STD_OPTIONAL

#endif //BSC_CEREAL_INCLUDE_H
