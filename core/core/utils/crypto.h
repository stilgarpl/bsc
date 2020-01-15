//
// Created by stilgar on 11.12.17.
//

#ifndef BASYCO_CRYPTO_H
#define BASYCO_CRYPTO_H


#include <filesystem>

namespace bsc {
    namespace fs = std::filesystem;

    std::string calculateSha1OfFile(fs::path path);

    std::string calculateSha1OfString(const std::string& string);

}

#endif //BASYCO_CRYPTO_H
