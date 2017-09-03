//
// Created by stilgar on 29.08.17.
//

#ifndef BASYCO_AUTHEVENT_H
#define BASYCO_AUTHEVENT_H


#include "../../../../logic/IEvent.h"
#include <string>


class AuthEvent : public IEvent<int> {

private:
    std::string login;
    std::string password;

public:
    const std::string &getLogin() const;

    void setLogin(const std::string &login);

    const std::string &getPassword() const;

    void setPassword(const std::string &password);
};


#endif //BASYCO_AUTHEVENT_H
