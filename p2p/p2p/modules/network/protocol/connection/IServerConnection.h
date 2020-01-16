//
// Created by stilgar on 09.10.17.
//

#ifndef BASYCO_ISERVERCONNECTION_H
#define BASYCO_ISERVERCONNECTION_H


namespace bsc {
    class IServerConnection : public bsc::Connection {

    public:
        IServerConnection(Context::Ptr context) : Connection(context) {}

        virtual void stop() = 0;

    };
}


#endif //BASYCO_ISERVERCONNECTION_H
