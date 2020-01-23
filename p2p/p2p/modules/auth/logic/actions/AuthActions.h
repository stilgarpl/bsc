//
// Created by stilgar on 19.07.18.
//

#ifndef BSC_AUTHACTIONS_H
#define BSC_AUTHACTIONS_H


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


#endif //BSC_AUTHACTIONS_H
