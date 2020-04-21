//
// Created by stilgar on 09.04.2020.
//

#include "MimeFileTypeDecoder.h"
//@todo if IO don't need logger, I can drop core dependency on io
#include <core/log/Logger.h>
#include <magic.h>
#include <mutex>

namespace bsc {
    using namespace std::string_literals;
    namespace detail {
        class FileTypeDecoderImplementation {
        private:
            magic_t magic_cookie;
            std::mutex magicLock;

            FileTypeDecoderImplementation(const FileTypeDecoderImplementation&) = delete;

            FileTypeDecoderImplementation(FileTypeDecoderImplementation&&) = delete;

            FileTypeDecoderImplementation() {
                std::lock_guard g(magicLock);
                magic_cookie = magic_open(MAGIC_MIME);
                if (magic_cookie == nullptr) {
                    ERROR("unable to initialize magic library");
                    // throw exception
                }
                if (magic_load(magic_cookie, nullptr) != 0) {
                    ERROR("cannot load magic database - "s + magic_error(magic_cookie));
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
                    //@todo throw exception
                }
                return result;
            }
        };
    }// namespace detail

    MimeFileType MimeFileTypeDecoder::getTypeForFile(const fs::path& path) {

        auto& decoderInstance = detail::FileTypeDecoderImplementation::instance();
        return MimeFileType::make(decoderInstance.getMime(path));
    }
}// namespace bsc
