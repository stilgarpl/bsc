//
// Created by stilgar on 27.08.17.
//

#include "AuthSource.h"

void AuthSource::auth(std::string login, std::string password) {
    auto event = std::make_shared<AuthEvent>();
    event->setLogin(login);
    event->setPassword(password);
    queueEvent(event);
}
