//
// Created by stilgar on 04.04.19.
//

#ifndef BASYCO_STRATEGY_H
#define BASYCO_STRATEGY_H

namespace bsc {
    enum class Strategy {

        PULL, //pulls file into the repository
        OVERWRITE, //overwrites file in the filesystem with the file from the repository
        DELETE, //deletes file
        TRASH, //trashes file
        ADD, //adds new file ?


    };
}

#endif //BASYCO_STRATEGY_H
