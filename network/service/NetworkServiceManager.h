//
// Created by stilgar on 02.08.17.
//

#ifndef BASYCO_NETWORKSERVICEMANAGER_H
#define BASYCO_NETWORKSERVICEMANAGER_H


#include <memory>

class NetworkServiceManager {
public:

    template<typename T>
    std::shared_ptr<T> operator()(void) {
        return findService<T>();
    };


    template<typename T>
    std::shared_ptr<T> &operator()(std::shared_ptr<T> &ptr) {
        return ptr = findService<T>();
    };

//    template<typename T>
//    std::weak_ptr<T>& operator()(std::weak_ptr<T>& ptr) {
//        return ptr = findService<T>();
//    };

public:
    template<typename T>
    std::shared_ptr<T> findService() {
        std::shared_ptr<T> &p = getService<T>();

        if (p == nullptr) {
            p = std::make_shared<T>();
            //@todo initialization or sth
        }
        return p;

    }

private:
    template<typename T>
    std::shared_ptr<T> &getService() {
        static std::shared_ptr<T> ptr;
        return ptr;
    }

public:
    static NetworkServiceManager &instance() {
        static NetworkServiceManager n;
        return n;
    }

private:

    NetworkServiceManager() {}
};

template<typename T>

std::shared_ptr<T> nsm(std::shared_ptr<T> &p) {
    return NetworkServiceManager::instance().findService<T>();
}
#endif //BASYCO_NETWORKSERVICEMANAGER_H
