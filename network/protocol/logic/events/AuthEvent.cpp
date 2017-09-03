//
// Created by stilgar on 29.08.17.
//

#include "AuthEvent.h"

const std::string &AuthEvent::getLogin() const {
    return login;
}

void AuthEvent::setLogin(const std::string &login) {
    AuthEvent::login = login;
}

const std::string &AuthEvent::getPassword() const {
    return password;
}

void AuthEvent::setPassword(const std::string &password) {
    AuthEvent::password = password;
}
