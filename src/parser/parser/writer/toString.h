//
// Created by stilgar on 14.06.2021.
//

#ifndef BSC_TOSTRING_H
#define BSC_TOSTRING_H
#include <parser/concepts/concepts.h>
#include <parser/configuration/ParserConfiguration.h>
#include <magic_enum.hpp>
#include <memory>
#include <numeric>
#include <algorithm>
#include <ranges>
namespace bsc {

    template<typename T>
    StringType toString(T&& t) ;

            class Writer {
    private:
        std::shared_ptr<ParserConfiguration> parserConfiguration = std::make_shared<ParserConfiguration>();

    public:
        explicit Writer(const ParserConfiguration& parserConfiguration) : parserConfiguration(std::make_shared<ParserConfiguration>(parserConfiguration)) {}
        Writer() = default;


        template<HasStdToString T>
        StringType toString(const T& t) const {
            return std::to_string(t);
        }

        template<IsString T>
        StringType toString(T t) const {
            return t;
        }

        template<IsStringView T>
        StringType toString(T t) const {
            return static_cast<StringType>(t);
        }

        template<IsEnum T>
        StringType toString(T t) const {
            return static_cast<StringType>(magic_enum::enum_name(t));
        }

        template<IsContainerNotString T>
        StringType toString(const T& tContainer) {
            using namespace std::string_literals;
            //@todo maybe elements should be joined by separator? like in Parser?
            auto transformed = tContainer | std::views::transform([this](auto&& i) {
                                   return this->toString(i);
                               }) | std::views::common;
            return std::accumulate(std::begin(transformed), std::end(transformed), ""s);
        }
    };

    template<typename T>
    StringType toString(T&& t) {
        static Writer writer;
        return writer.template toString(std::forward<T>(t));
    }

}

#endif// BSC_TOSTRING_H
