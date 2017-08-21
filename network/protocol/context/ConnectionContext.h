//
// Created by stilgar on 21.08.17.
//

#ifndef BASYCO_CONNECTIONCONTEXTINFO_H
#define BASYCO_CONNECTIONCONTEXTINFO_H


#include "../connection/Connection.h"
#include "IContextSetup.h"

class ConnectionContext {
public:
    class Setup : public IContextSetup {
        Connection &connection;
    public:
        Setup(Connection &connection);

    public:
        void setup(Context &context) override;
    };

private:
    Connection &connection;


public:
    ConnectionContext(Connection &connection);

    Connection &getConnection() const;

    int test;


};


#endif //BASYCO_CONNECTIONCONTEXTINFO_H
