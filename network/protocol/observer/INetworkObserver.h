//
// Created by stilgar on 27.08.17.
//

#ifndef BASYCO_INETWORKOBSERVER_H
#define BASYCO_INETWORKOBSERVER_H


#include "../connection/Connection.h"

class INetworkObserver {
public:

    virtual void notifyNewConnection(Connection *)=0;

    virtual void notifyClosedConnection(Connection *)=0;
};


#endif //BASYCO_INETWORKOBSERVER_H
