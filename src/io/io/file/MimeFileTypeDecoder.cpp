//
// Created by Krzysztof Tulidowicz on 09.04.2020.
//

#include "MimeFileTypeDecoder.h"
//@todo if IO don't need logger, I can drop core dependency on io
#include <fmt/format.h>
#include <log/log/Logger.h>
#include <magic.h>
#include <mutex>

namespace bsc {

    class MimeFileTypeDecodingException : public std::domain_error {
    public:
        MimeFileTypeDecodingException(const std::string& arg) : domain_error(arg) {}
    };

    using namespace std::string_literals;
    namespace detail {
        class FileTypeDecoderImplementation {
        private:
            magic_t magic_cookie;
            std::mutex magicLock;

        public:
            FileTypeDecoderImplementation(const FileTypeDecoderImplementation&) = delete;

            FileTypeDecoderImplementation(FileTypeDecoderImplementation&&) = delete;

        private:
            FileTypeDecoderImplementation() {
                std::lock_guard g(magicLock);
                magic_cookie = magic_open(MAGIC_MIME);
                if (magic_cookie == nullptr) {
                    logger.error("unable to initialize magic library");
                    // throw exception
                }
                if (magic_load(magic_cookie, nullptr) != 0) {
                    logger.error("cannot load magic database - "s + magic_error(magic_cookie));
                    magic_close(magic_cookie);
                    // throw exception
                }
            }

        public:
            static FileTypeDecoderImplementation& instance() {
                static FileTypeDecoderImplementation self;
                return self;
            }

            virtual ~FileTypeDecoderImplementation() { magic_close(magic_cookie); }

            std::string getMime(const fs::path& path) {
                std::lock_guard g(magicLock);
                auto result = magic_file(magic_cookie, path.c_str());
                if (result == nullptr) {
                    throw MimeFileTypeDecodingException(fmt::format("Unable to match mime for path: {}", path.string()));
                }
                return result;
            }
        };
    }// namespace detail

    MimeFileType MimeFileTypeDecoder::getTypeForFile(const fs::path& path) {

        auto& decoderInstance = detail::FileTypeDecoderImplementation::instance();
        return MimeFileTypeFactory().create(decoderInstance.getMime(path));
    }
}// namespace bsc
