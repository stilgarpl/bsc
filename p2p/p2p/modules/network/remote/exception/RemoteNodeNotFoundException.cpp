//
// Created by stilgar on 07.07.18.
//

#include "RemoteNodeNotFoundException.h"

bsc::RemoteNodeNotFoundException::RemoteNodeNotFoundException(const std::string& arg) : domain_error(arg) {}
