//
// Created by Krzysztof Tulidowicz on 09.10.17.
//

#ifndef BSC_ISERVERCONNECTION_H
#define BSC_ISERVERCONNECTION_H


namespace bsc {
    class IServerConnection : public bsc::Connection {

    public:
        IServerConnection(Context::Ptr context) : Connection(context) {}

        virtual void stop() = 0;

    };
}


#endif //BSC_ISERVERCONNECTION_H
