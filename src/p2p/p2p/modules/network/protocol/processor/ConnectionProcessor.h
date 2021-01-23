//
// Created by Krzysztof Tulidowicz on 20.08.17.
//

#ifndef BSC_CONNECTIONPROCESSOR_H
#define BSC_CONNECTIONPROCESSOR_H

#include "context/context/Context.h"
#include "core/log/Logger.h"
#include "core/thread/Runnable.h"
#include <p2p/modules/network/protocol/filter/PacketFilter.h>
#include <p2p/modules/network/protocol/filter/RoleFilter.h>
#include <thread>

namespace bsc {
    class Connection;

    class ConnectionProcessor : public bsc::Runnable {

    private:
        Logger logger = Logger("Connection Processor");

    protected:
        Connection& connection;
        //@todo initialize packet filter from AuthModule or configuration or sth.
        std::unique_ptr<PacketFilter> packetFilter = std::make_unique<RoleFilter>();

        void run() override;

    public:
        explicit ConnectionProcessor(Connection& connection);

        ~ConnectionProcessor() = default;

    };
}


#endif //BSC_CONNECTIONPROCESSOR_H
