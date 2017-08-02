//
// Created by stilgar on 02.08.17.
//

#ifndef BASYCO_NETWORKSERVICEMANAGER_H
#define BASYCO_NETWORKSERVICEMANAGER_H


#include <memory>

class NetworkServiceManager {

    template<typename T>
    void operator()(std::shared_ptr<T> ptr) {
///@TODO implement
    };

    template<typename T>
    std::shared_ptr<T> findService() {
        ///@todo implement
        return new T();
    }


};

typedef NetworkServiceManager nsm;
#endif //BASYCO_NETWORKSERVICEMANAGER_H
