//
// Created by Krzysztof Tulidowicz on 08.01.2020.
//

#ifndef BSC_FROMSTRING_H
#define BSC_FROMSTRING_H

#include <limits>
#include <memory>
#include <parser/concepts/concepts.h>
#include <parser/configuration/ParserConfiguration.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
//@todo make it optional on cmake option to use magic_enum
#include <magic_enum.hpp>

#include "detail/date.h"
#include "detail/tz.h"

namespace bsc {

    class StringParseException : public std::invalid_argument {
    public:
        explicit StringParseException(const StringType& arg) : invalid_argument(arg) {
        }
    };

    class Parser;

    namespace detail {
        struct DefaultParserImplementation {

            Parser* parser;
            //@todo parserConfiguration can be removed since we have parser reference
            std::shared_ptr<ParserConfiguration> parserConfiguration = std::make_shared<ParserConfiguration>();

        public:
            explicit DefaultParserImplementation(Parser* p, std::shared_ptr<ParserConfiguration> parserConfiguration)
                : parser(p), parserConfiguration(std::move(parserConfiguration)) {
            }

        public:// I'd prefer this to be private, but concept HasDefaultImplementation does not work with private (it doesn't see friend)
            friend class Parser;

            template<typename ParameterType>
            [[nodiscard]] std::remove_reference_t<ParameterType> fromStringImpl(const StringType& value) const
                requires std::is_floating_point_v<ParameterType>
            {
                try {
                    //@todo use "if constexpr" to determine the size of ParameterType and use appropriate std function
                    return std::stod(value);
                } catch (const std::invalid_argument& e) {
                    throw StringParseException("Floating parsing failed for value: " + value);
                }
            }

            template<IsBool ParameterType>
            [[nodiscard]] bool fromStringImpl(const StringType& value) const {
                //@todo maybe check if string is not empty and then if it has values like "false" or something -- this could be selected by
                //ParserConfiguration flag
                return true;
            }

            template<typename ParameterType>
            [[nodiscard]] std::remove_reference_t<ParameterType> fromStringImpl(const StringType& value) const
                requires std::is_integral_v<ParameterType> && (!IsBool<ParameterType>)
            {
                try {
                    //@todo use "if constexpr" to determine the size of ParameterType and use appropriate std function
                    return std::stoll(value);
                } catch (const std::invalid_argument& e) {
                    throw StringParseException("Integer parsing failed for value: " + value);
                }
            }

            template<typename ParameterType>
            [[nodiscard]] std::remove_reference_t<ParameterType> fromStringImpl(const StringType& value) const
                requires std::is_enum_v<ParameterType>
            {
                auto optionalEnum = magic_enum::enum_cast<ParameterType>(value);
                if (optionalEnum) {
                    return *optionalEnum;
                } else {
                    throw StringParseException("Enum parsing failed for value: " + value);
                }
            }

            template<typename ParameterType>
            [[nodiscard]] std::remove_reference_t<ParameterType> fromStringImpl(const StringType& value) const
                requires std::is_convertible_v<ParameterType, StringType>
            {
                if (!value.empty()) {
                    return ParameterType(value);
                } else {
                    throw StringParseException("Can't parse null parameter value");
                }
            }

            template<IsPair ParameterType>
            [[nodiscard]] std::remove_reference_t<ParameterType> fromStringImpl(const StringType& value) const {
                try {
                    std::stringstream inputStream(value);
                    StringType first, second;
                    getline(inputStream, first, parserConfiguration->pairDelimiter);
                    getline(inputStream, second);
                    auto key       = fromString<std::decay_t<typename ParameterType::first_type>>(first.c_str());
                    auto pairValue = fromString<typename ParameterType::second_type>(second.c_str());
                    return std::make_pair(key, pairValue);
                } catch (const std::invalid_argument& e) {
                    throw StringParseException("Pair parsing failed for value: " + value);
                }
            }

            template<IsContainerNotString ParameterType>
            [[nodiscard]] std::remove_reference_t<ParameterType> fromStringImpl(const StringType& value) const;

            template<typename Duration>
            static const char* selectTimeFormatPrecision() {
                //@todo maybe move it somewhere else
                if (auto dur = Duration{1}; dur >= std::chrono::years{1}) {
                    return "%Y";
                } else if (dur >= std::chrono::months{1}) {
                    return "%Y-%m";
                } else if (dur >= std::chrono::days{1}) {
                    return "%F";
                } else if (dur >= std::chrono::minutes{1}) {
                    return "%F %R";
                } else if (dur >= std::chrono::seconds{1}) {
                    return "%F %T";
                }

                return "%c";
            }

            template<IsTimePoint ParameterType>
            [[nodiscard]] std::remove_reference_t<ParameterType> fromStringImpl(const StringType& value, std::string format = selectTimeFormatPrecision<typename ParameterType::duration>()) const {
                //@todo C++23 C++26 change format to string_view when chrono::parse supports it...
                //chrono date library is stupid with incomplete dates, so...
                //@todo try to make it less ugly
                using SystemTimePoint = std::chrono::sys_time<std::chrono::nanoseconds>;
                {
                    std::stringstream ss{value};
                    SystemTimePoint result;
                    //parsing full timepoint
                    ss >> date::parse(format, result);
                    if (!ss.fail()) return std::chrono::time_point_cast<typename ParameterType::duration> (date::clock_cast<typename ParameterType::clock>(result));
                }
                {
                    std::stringstream ss{value};
                    //parsing incomplete date (year and month)
                    using namespace date;
                    date::year_month ym{};
                    ss >> date::parse(format, ym);
                    date::year_month_day tempYmd = ym / 1;
                    std::chrono::sys_days tempSd = tempYmd.operator sys_days();
                    if (!ss.fail()) {
                        return std::chrono::time_point_cast<typename ParameterType::duration> (date::clock_cast<typename ParameterType::clock>(tempSd));
                    }
                }
                {
                    std::stringstream ss{value};
                    //parsing incomplete date (year)
                    using namespace date;
                    date::year y{};
                    ss >> date::parse(format, y);
                    date::year_month_day tempYmd = y / 1 / 1;
                    if (!ss.fail()) {
                        return std::chrono::time_point_cast<typename ParameterType::duration> (date::clock_cast<typename ParameterType::clock>(tempYmd.operator sys_days()));
                    }
                }
                    using namespace std::string_literals;
                    throw StringParseException("Unable to parse date with format: "s + format);

            }
        };

        template<typename T, typename ... Args>
        concept HasDefaultParserImplementation =
                requires(const DefaultParserImplementation& parser, const StringType& s, Args ... args ) {
                    {parser.fromStringImpl<T>(s, args...)} -> std::same_as<std::remove_reference_t<T>>;
                };
    }// namespace detail

    template<typename ParameterType>
    class ParserImplementation {
    public:
        explicit ParserImplementation(const Parser&){ };
        //        [[nodiscard]] std::remove_reference_t<ParameterType>
        //        parse(const StringType& value) {
        //
        //        }
    };

    template<typename ParameterType, typename ... Args>
    concept HasCustomParserImplementation = requires(ParserImplementation<ParameterType> p, StringType s, Args ... args) {
                                                { p.parse(s, args...) } -> std::same_as<std::remove_reference_t<ParameterType>>;
                                            };

    class Parser {
    private:
        std::shared_ptr<ParserConfiguration> parserConfiguration = std::make_shared<ParserConfiguration>();
        detail::DefaultParserImplementation defaultParserImplementation{this, parserConfiguration};

    public:
        explicit Parser(const ParserConfiguration& parserConfiguration)
            : parserConfiguration(std::make_shared<ParserConfiguration>(parserConfiguration)) {
        }
        Parser() = default;
        template<typename ParameterType, typename ... Args>
        [[nodiscard]] std::remove_cvref_t<ParameterType> fromString(StringType value, Args... args) const
            requires detail::HasDefaultParserImplementation<std::remove_cvref_t<ParameterType>, Args...>
                     || HasCustomParserImplementation<std::remove_cvref_t<ParameterType>, Args...>
        {
            if constexpr (HasCustomParserImplementation<std::remove_cvref_t<ParameterType>>) {
                ParserImplementation<ParameterType> impl(*this);
                return impl.parse(value, args...);
            } else {
                return defaultParserImplementation.fromStringImpl<std::remove_cvref_t<ParameterType>>(std::move(value), args...);
            }
        }
    };

    template<typename ParameterType>
    [[nodiscard]] auto fromString(const StringType& value, auto... args) {
        static Parser parser;
        return parser.fromString<ParameterType>(value);
    }

    template<typename T>
    concept ParsedFromString =
            requires(const Parser& parser, const StringType& s) { parser.fromString<T>(s); } || HasCustomParserImplementation<T>;

    namespace detail {
        template<IsContainerNotString ParameterType>
        std::remove_reference_t<ParameterType> DefaultParserImplementation::fromStringImpl(const StringType& value) const {
            ParameterType container;
            std::stringstream inputStream(value);
            StringType element;

            while (getline(inputStream, element, parserConfiguration->csvDelimiter)) {
                container.insert(container.end(), parser->fromString<typename ParameterType::value_type>(element.c_str()));
            }

            return container;
        }
    }// namespace detail

}// namespace bsc

#endif// BSC_FROMSTRING_H
