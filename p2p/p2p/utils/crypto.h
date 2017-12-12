//
// Created by stilgar on 11.12.17.
//

#ifndef BASYCO_CRYPTO_H
#define BASYCO_CRYPTO_H


#include <cryptopp/files.h>
#include <cryptopp/sha.h>

#include <experimental/filesystem>
#include <cryptopp/hex.h>

namespace fs = std::experimental::filesystem;

std::string calculateSha1OfFile(fs::path path);

#endif //BASYCO_CRYPTO_H
