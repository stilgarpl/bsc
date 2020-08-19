//
// Created by Krzysztof Tulidowicz on 21.08.17.
//

#ifndef BSC_STATUS_H
#define BSC_STATUS_H

namespace bsc {
    //@todo consider renaming this to something with better name PacketType? PacketSth? ...
    enum class Status {
        request,
        response,
        ack,
        error,
    };
}


#endif //BSC_STATUS_H
