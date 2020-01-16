//
// Created by stilgar on 10.06.19.
//

#include "RemoteNodeConnectionException.h"

bsc::RemoteNodeConnectionException::RemoteNodeConnectionException(const std::string& arg)
        : domain_error(arg) {}
