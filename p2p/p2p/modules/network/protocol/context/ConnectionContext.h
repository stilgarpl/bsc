//
// Created by Krzysztof Tulidowicz on 21.08.17.
//

#ifndef BSC_CONNECTIONCONTEXTINFO_H
#define BSC_CONNECTIONCONTEXTINFO_H


#include <p2p/modules/network/protocol/connection/Connection.h>


namespace bsc {
    class ConnectionContext {
    public:

    private:
        Connection& connection;


    public:
        explicit ConnectionContext(Connection& connection);

        Connection& getConnection() const;


    };
}


#endif //BSC_CONNECTIONCONTEXTINFO_H
