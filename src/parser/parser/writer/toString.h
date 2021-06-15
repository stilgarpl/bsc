//
// Created by stilgar on 14.06.2021.
//

#ifndef BSC_TOSTRING_H
#define BSC_TOSTRING_H
#include <parser/concepts/concepts.h>
#include <parser/configuration/ParserConfiguration.h>

namespace bsc {

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

    };

}

#endif// BSC_TOSTRING_H
