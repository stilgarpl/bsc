//
// Created by Krzysztof Tulidowicz on 25.03.22.
//
#ifndef BSC_MODIFIERAFFIXES_H
#define BSC_MODIFIERAFFIXES_H

#include "Affix.h"
#include <functional>
#include <map>

namespace bsc {
    namespace affixes {
        template<IsAffix AffixType, auto transformator>
        struct TransformAffix : public AffixType {
            template<typename T>
            auto transform(const T& t) const {
                if (this->value.has_value()) {
                    return transformator(*this->value, t);
                } else {
                    throw EmptyAffixException("Affix is empty");
                }
            }
        };


    }// namespace affixes
}// namespace bsc

#endif// BSC_MODIFIERAFFIXES_H
