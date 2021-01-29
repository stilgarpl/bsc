//
// Created by Krzysztof Tulidowicz on 21.01.2021.
//

#ifndef BSC_PROPERTYPARSERNODETYPE_H
#define BSC_PROPERTYPARSERNODETYPE_H

namespace bsc {

    enum class PropertyParserNodeType {
        invalid,
        empty,
        scalar,
        sequence,
        map,
    };
}

#endif// BSC_PROPERTYPARSERNODETYPE_H
