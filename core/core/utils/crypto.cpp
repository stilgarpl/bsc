//
// Created by stilgar on 11.12.17.
//

#include "crypto.h"

std::string calculateSha1OfFile(fs::path path) {
    CryptoPP::SHA256 hash;
    std::string digest;
//    auto size = fs::file_size(fs::path(path));
    CryptoPP::FileSource f(path.c_str(), true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(
            new CryptoPP::StringSink(digest))));
    return digest;
}
