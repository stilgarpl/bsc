
find_package(LibMagic REQUIRED)
find_package(exiv2 CONFIG REQUIRED)
find_package(inja CONFIG REQUIRED)

set(SOURCE_FILES io/file/MimeFileTypeDecoder.cpp io/file/MimeFileTypeDecoder.h io/file/MimeFileType.h io/file/MimeFileType.cpp io/translator/PropertiesMetaData.cpp io/translator/PropertiesMetaData.h io/file/FileMetaDataReader.cpp io/file/FileMetaDataReader.h io/file/reader/GenericFileReader.cpp io/file/reader/GenericFileReader.h io/file/reader/FileReaderAlgorithm.h io/file/reader/ImageFileReader.cpp io/file/reader/ImageFileReader.h io/sorter/FileSorter.cpp io/sorter/FileSorter.h io/sorter/mappers/FileSorterMapper.h io/sorter/mappers/FileSorterMapper.cpp io/sorter/fetchers/FileListFetcher.h io/sorter/fetchers/FileListFetcher.cpp io/sorter/fetchers/FilesystemFileListFetcher.cpp io/sorter/fetchers/FilesystemFileListFetcher.h io/sorter/fetchers/StaticFileListFetcher.cpp io/sorter/fetchers/StaticFileListFetcher.h io/sorter/actions/StandardFileSorterActions.cpp io/sorter/actions/StandardFileSorterActions.h io/sorter/mappers/FileSorterMapperMatcher.h io/sorter/mappers/FileSorterMimeMatcher.cpp io/sorter/mappers/FileSorterMimeMatcher.h io/translator/PathTranslator.cpp io/translator/PathTranslator.h io/sorter/fetchers/FileListFetcher.cpp io/sorter/mappers/FileSorterNameMatcher.cpp io/sorter/mappers/FileSorterNameMatcher.h)

add_library(io ${SOURCE_FILES})
target_include_directories(io PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:${BSC_DESTINATION_INCLUDE}>)
target_precompile_headers(io PRIVATE
        <filesystem>
        )
install(DIRECTORY io DESTINATION ${BSC_DESTINATION_INCLUDE}
        FILES_MATCHING PATTERN "*.h")
target_link_libraries(io PUBLIC LibMagic::LibMagic core exiv2 parser)
install(TARGETS io COMPONENT io DESTINATION ${BSC_DESTINATION_LIB} EXPORT ${PROJECT_NAME}Targets)
if (${BSC_ENABLE_TESTS})
    add_subdirectory(test)
endif ()