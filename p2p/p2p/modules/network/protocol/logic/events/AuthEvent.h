//
// Created by stilgar on 29.08.17.
//

#ifndef BSC_AUTHEVENT_H
#define BSC_AUTHEVENT_H


#include "logic/IEvent.h"

#include <string>


namespace bsc {
    class AuthEvent : public bsc::IEvent<int> {

    private:
        std::string login;
        std::string password;

    public:
        const std::string& getLogin() const;

        void setLogin(const std::string& login);

        const std::string& getPassword() const;

        void setPassword(const std::string& password);
    };
}


#endif //BSC_AUTHEVENT_H
