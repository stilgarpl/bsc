//
// Created by Krzysztof Tulidowicz on 29.03.22.
//
#ifndef BSC_DATETIMEVALUE_H
#define BSC_DATETIMEVALUE_H

#include "optional/optional.h"
#include "parser/configuration/ParserConfiguration.h"
#include "parser/parser/fromString.h"
#include <algorithm>
#include <chrono>
#include <variant>
#include <fmt/chrono.h>
#include <fmt/format.h>
namespace bsc {

    namespace detail {
        enum class Precision { years, months, days, minutes, seconds, nanoseconds };

        constexpr static auto selectTimePrecisionFormat(Precision precision) {
            using enum Precision;
            switch (precision) {
                case years:
                    return "%Y";
                case months:
                    return "%Y-%m";
                case days:
                    return "%Y-%m-%d";
                case minutes:
                    return "%F %R";
                case seconds:
                    return "%F %T";
                default:
                    return "%c";
            }
        }

    }


    class DateTimeValue {

    public:

        using Precision = detail::Precision;

    private:
        Precision precision;
        std::chrono::time_point<std::chrono::system_clock> timePoint;

    public:
        template<typename Clock, typename Duration>
        explicit DateTimeValue(const std::chrono::time_point<Clock, Duration>& timePoint, Precision precision = Precision::nanoseconds)
            : precision(precision), timePoint(date::clock_cast<std::chrono::system_clock>(timePoint)) {
        }

        std::strong_ordering operator<=>(const DateTimeValue& other) const {
            auto lowestPrecision = std::min(precision, other.precision);
            std::chrono::year_month_day thisYmd(std::chrono::floor<std::chrono::days>(this->timePoint));
            std::chrono::year_month_day otherYmd(std::chrono::floor<std::chrono::days>(other.timePoint));
            std::chrono::hh_mm_ss thisHms(this->timePoint - std::chrono::floor<std::chrono::days>(this->timePoint));
            std::chrono::hh_mm_ss otherHms(other.timePoint - std::chrono::floor<std::chrono::days>(other.timePoint));
            using enum Precision;

            auto dateSo   = thisYmd <=> otherYmd;
            auto yearSo   = thisYmd.year() <=> otherYmd.year();
            auto monthSo  = thisYmd.month() <=> otherYmd.month();
            auto daySo    = thisYmd.day() <=> otherYmd.day();

            auto hourSo   = thisHms.hours() <=> otherHms.hours();
            auto minuteSo = thisHms.minutes() <=> otherHms.minutes();
            auto secondSo = thisHms.seconds() <=> otherHms.seconds();

            switch (lowestPrecision) {

                case seconds :
                    if (dateSo == 0 && hourSo == 0 && minuteSo == 0)
                        return secondSo;
                    [[fallthrough]];
                case minutes:
                    if (dateSo == 0 && hourSo == 0)
                        return minuteSo;
                    else if (dateSo == 0)
                        return hourSo;
                    [[fallthrough]];
                case days:
                    if (yearSo == 0 && monthSo == 0)
                        return daySo;
                    [[fallthrough]];
                case months:
                    if (yearSo == 0)
                        return monthSo;
                    [[fallthrough]];
                case years:
                    return yearSo;

                default:
                    return this->timePoint <=> other.timePoint;
            }
        }

        bool operator==(const DateTimeValue& other) const {
            return (*this <=> other) == 0;
        }

        friend std::ostream& operator<<(std::ostream& os, const DateTimeValue& self) {
            using namespace std::string_literals;
            os << fmt::format(fmt::runtime("{0:"s+detail::selectTimePrecisionFormat(self.precision)+"}"), self.timePoint);
            return os;
        }
    };

        template<>
        class ParserImplementation<DateTimeValue> {
        private:
            using DTV = DateTimeValue;
            const Parser& parser;

        private:





            std::optional<DTV> tryParse(const StringType& value, DateTimeValue::Precision precision) {
                try {
                    return DTV{parser.template fromString<std::chrono::system_clock::time_point>(value, selectTimePrecisionFormat(precision)), precision};
                } catch (const StringParseException& e) {
                    return std::nullopt;
                }
            }

        public:
            explicit ParserImplementation(const Parser& parser) : parser(parser){};

            [[nodiscard]] std::remove_reference_t<DTV> parse(const StringType& value) {
                using enum DateTimeValue::Precision;
                return tryParse(value, seconds)
                       | optional::orElseChainGet([this, &value] { return tryParse(value, minutes); })
                       | optional::orElseChainGet([this, &value] { return tryParse(value, days); })
                       | optional::orElseChainGet([this, &value] { return tryParse(value, months); })
                       | optional::orElseChainGet([this, &value] { return tryParse(value, years); })
                       | optional::orElseThrow(StringParseException("Unable to parse DateTimeValue"));
            }
        };
}// namespace bsc

#endif