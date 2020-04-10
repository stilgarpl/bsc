//
// Created by stilgar on 09.04.2020.
//

#include "FileTypeDecoder.h"
#include <magic.h>

namespace bsc {
    using namespace std::string_literals;
    namespace detail {
        class FileTypeDecoderImplementation {
        private:
            magic_t magic_cookie;

            FileTypeDecoderImplementation(const FileTypeDecoderImplementation &) = delete;

            FileTypeDecoderImplementation(FileTypeDecoderImplementation &&) = delete;

            FileTypeDecoderImplementation() {
                magic_cookie = magic_open(MAGIC_MIME);
                if (magic_cookie == NULL) {
                    ERROR("unable to initialize magic library");
                    // throw exception
                }
                if (magic_load(magic_cookie, NULL) != 0) {
                    ERROR("cannot load magic database - "s + magic_error(magic_cookie));
                    magic_close(magic_cookie);
                    // throw exception
                }

            }

        public:

            static FileTypeDecoderImplementation &instance() {
                static FileTypeDecoderImplementation self;
                return self;
            }


            virtual ~FileTypeDecoderImplementation() {
                magic_close(magic_cookie);
            }

            std::string getMime(const fs::path &path) {
                auto result = magic_file(magic_cookie, path.c_str());
                if (result == NULL) {
                    //@todo throw exception
                }
                return result;
            }
        };
    }
}

bsc::FileType bsc::FileTypeDecoder::getTypeForFile(const fs::path &path) {

    auto &decoderInstance = detail::FileTypeDecoderImplementation::instance();
    return FileType::make(decoderInstance.getMime(path));


}
