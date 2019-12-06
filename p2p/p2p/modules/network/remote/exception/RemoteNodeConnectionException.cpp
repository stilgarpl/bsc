//
// Created by stilgar on 10.06.19.
//

#include "RemoteNodeConnectionException.h"

RemoteNodeConnectionException::RemoteNodeConnectionException(const std::string& arg)
        : domain_error(arg) {}
