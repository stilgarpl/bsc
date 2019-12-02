//
// Created by stilgar on 11.12.17.
//

#include <Poco/SHA1Engine.h>
#include <Poco/DigestStream.h>
#include <fstream>
#include "crypto.h"

std::string calculateSha1OfFile(fs::path path) {

    Poco::SHA1Engine sha1Engine;
    std::ifstream file(path, std::ios::binary);
    if (file.is_open()) {
        Poco::DigestInputStream digestStream(sha1Engine, file);
        size_t bufSize = 0x100000;
        char* buf = new char[bufSize];
        digestStream.read(buf, bufSize);
        while ((digestStream.rdstate() & std::ifstream::failbit) == 0) {
            digestStream.read(buf, bufSize);
        }
        delete[] buf;
        if ((digestStream.rdstate() & std::ifstream::eofbit) != 0) {

            return Poco::DigestEngine::digestToHex(sha1Engine.digest());
        }

    }

//    CryptoPP::SHA256 hash;
//    std::string digest;
////    auto size = fs::file_size(fs::path(path));
//    CryptoPP::FileSource f(path.c_str(), true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(
//            new CryptoPP::StringSink(digest))));
    return ""; //@todo fixme return failure exception or sth.
}

std::string calculateSha1OfString(const std::string& source) {
    Poco::SHA1Engine sha1Engine;
    sha1Engine.update(source);
    return Poco::SHA1Engine::digestToHex(sha1Engine.digest());
}
