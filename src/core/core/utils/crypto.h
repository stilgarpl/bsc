//
// Created by Krzysztof Tulidowicz on 11.12.17.
//

#ifndef BSC_CRYPTO_H
#define BSC_CRYPTO_H


#include <filesystem>

namespace bsc {
    namespace fs = std::filesystem;

    std::string calculateSha1OfFile(fs::path path);

    std::string calculateSha1OfString(const std::string& string);

}

#endif //BSC_CRYPTO_H
