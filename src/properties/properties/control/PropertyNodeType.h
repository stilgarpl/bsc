//
// Created by Krzysztof Tulidowicz on 21.01.2021.
//

#ifndef BSC_PROPERTYNODETYPE_H
#define BSC_PROPERTYNODETYPE_H

namespace bsc {

    enum class PropertyNodeType {
        invalid,
        empty,
        scalar,
        sequence,
        map,
    };
}

#endif// BSC_PROPERTYNODETYPE_H
