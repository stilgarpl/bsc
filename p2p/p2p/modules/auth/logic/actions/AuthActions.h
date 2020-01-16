//
// Created by stilgar on 19.07.18.
//

#ifndef BASYCO_AUTHACTIONS_H
#define BASYCO_AUTHACTIONS_H


#include <p2p/modules/auth/logic/events/AuthHelloEvent.h>


namespace bsc {
    class AuthActions {
    public:

        static void helloAction(const AuthHelloEvent& event) {
            //verify that event.nodeId is known
            //verify authData in authModule
        }

    };
}


#endif //BASYCO_AUTHACTIONS_H
