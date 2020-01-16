//
// Created by stilgar on 31.12.17.
//

#include <logic/context/LogicContext.h>
#include "PropertiesTransfer.h"


bsc::TransferId bsc::PropertiesTransfer::Request::getTransferId() const {
    return transferId;
}

void bsc::PropertiesTransfer::Request::setTransferId(TransferId transferId) {
    Request::transferId = transferId;
}
