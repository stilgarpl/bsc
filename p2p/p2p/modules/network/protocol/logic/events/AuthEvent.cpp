//
// Created by stilgar on 29.08.17.
//

#include "AuthEvent.h"

const std::string& bsc::AuthEvent::getLogin() const {
    return login;
}

void bsc::AuthEvent::setLogin(const std::string& login) {
    AuthEvent::login = login;
}

const std::string& bsc::AuthEvent::getPassword() const {
    return password;
}

void bsc::AuthEvent::setPassword(const std::string& password) {
    AuthEvent::password = password;
}
