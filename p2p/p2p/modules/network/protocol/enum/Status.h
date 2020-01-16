//
// Created by stilgar on 21.08.17.
//

#ifndef BASYCO_STATUS_H
#define BASYCO_STATUS_H

namespace bsc {
    //@todo consider renaming this to something with better name PacketType? PacketSth? ...
    enum class Status {
        request,
        response,
        ack,
        error,
    };
}


#endif //BASYCO_STATUS_H
