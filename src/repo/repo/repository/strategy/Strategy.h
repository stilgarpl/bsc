//
// Created by Krzysztof Tulidowicz on 04.04.19.
//

#ifndef BSC_STRATEGY_H
#define BSC_STRATEGY_H

namespace bsc {
    enum class Strategy {

        PULL, //pulls file into the repository
        OVERWRITE, //overwrites file in the filesystem with the file from the repository
        DELETE, //deletes file
        TRASH, //trashes file
        ADD, //adds new file ?


    };
}

#endif //BSC_STRATEGY_H
