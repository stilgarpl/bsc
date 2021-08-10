//
// Created by Krzysztof Tulidowicz on 09.08.2021.
//
#ifndef BSC_FLAGS_H
#define BSC_FLAGS_H

#include <set>
#include <ranges>
#include <functional>

namespace bsc {
    template<typename FlagType>
    class Flags {
        std::set<FlagType> flags{};

        class OptionsHelper {
            const Flags& options;
            bool value{};
            bool negative;

        public:
            operator bool() const {
                return !negative ? value : !value;
            };
            OptionsHelper(const Flags& o, bool value, bool negate=false) : options(o), value(value), negative(negate) {};
            OptionsHelper andHas(const FlagType& i) const {
                return OptionsHelper(options, value && options.has(i), negative);
            }

            OptionsHelper orHas(const FlagType& i) const{
                return OptionsHelper(options, value || options.has(i), negative);
            }

            OptionsHelper invert() const {
                return OptionsHelper(options, value, !negative);
            }

            OptionsHelper operator&&(const FlagType& i) const{
                return andHas(i);
            }

            OptionsHelper operator||(const FlagType& i) const{
                return orHas(i);
            }

            OptionsHelper operator!() const{
                return invert();
            }
        };

    public:
        void enable(const FlagType& i) {
            flags.insert(i);
        }

        void enable(const std::initializer_list<FlagType>& initializerList) {
            for (const auto& item : initializerList) {
                enable(item);
            }
        }

        void disable(const FlagType& i) {
            flags.erase(i);
        }

        void disable(const std::initializer_list<FlagType>& initializerList) {
            for (const auto& item : initializerList) {
                disable(item);
            }
        }

        OptionsHelper has(const FlagType& i) const {
            return OptionsHelper(*this, flags.contains(i));
        }

        OptionsHelper hasNot(const FlagType& i) const {
            return has(i).invert();
        }

        Flags() = default;
        Flags(const std::initializer_list<FlagType>& initializerList) : flags(initializerList) {
        }

        OptionsHelper operator==(const FlagType& i ) const {
            return this->has(i);
        }

        OptionsHelper operator!=(const FlagType& i ) const {
            return this->hasNot(i);
        }
    };
}// namespace bsc

#endif