//
// Created by stilgar on 31.07.17.
//

#ifndef BASYCO_APPLICATIONPROTOCOL_H
#define BASYCO_APPLICATIONPROTOCOL_H


#include <sstream>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include "TransferProtocol.h"


class ApplicationProtocol {

private:
    TransferProtocol &tp;

public:

    template<typename T>
    void send(const T &t) {
        std::stringstream s;
        {

            cereal::JSONOutputArchive outputArchive(s);
            outputArchive(t);

        }
        std::cout << "DEEEEBUG" << s.str() << std::endl;
    }

    ApplicationProtocol(TransferProtocol &tp) : tp(tp) {}

};


#endif //BASYCO_APPLICATIONPROTOCOL_H
