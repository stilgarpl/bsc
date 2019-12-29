//
// Created by stilgar on 31.12.17.
//

#include <logic/context/LogicContext.h>
#include "PropertiesTransfer.h"


TransferId PropertiesTransfer::Request::getTransferId() const {
    return transferId;
}

void PropertiesTransfer::Request::setTransferId(TransferId transferId) {
    Request::transferId = transferId;
}
