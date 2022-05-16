//
// Created by Krzysztof Tulidowicz on 25.03.22.
//
#ifndef BSC_SIAFFIX_H
#define BSC_SIAFFIX_H
#include "ModifierAffixes.h"
#include "StandardAffixes.h"
#include "constant/standard/World.h"
#include "math/arithmetic.h"
namespace bsc::affixes{

    using SiEnumPrefix = affixes::TransformAffix<affixes::EnumAffix<SiPrefix, true, SiPrefix::null>, []<typename EnumType>(EnumType v, auto i) {
        return math::pown(10ll, static_cast<std::underlying_type_t<EnumType>>(v)) * i;}>;
}

#endif// BSC_SIAFFIX_H
