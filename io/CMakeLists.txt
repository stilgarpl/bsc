
find_package(LibMagic REQUIRED)
find_package(exiv2 CONFIG REQUIRED)

set(SOURCE_FILES io/file/FileTypeDecoder.cpp io/file/FileTypeDecoder.h io/file/FileType.h io/file/FileType.cpp io/file/FileMetaData.cpp io/file/FileMetaData.h io/file/FileMetaDataReader.cpp io/file/FileMetaDataReader.h io/file/reader/GenericFileReader.cpp io/file/reader/GenericFileReader.h io/file/reader/FileReaderAlgorithm.h io/file/reader/ImageFileReader.cpp io/file/reader/ImageFileReader.h)

add_library(io ${SOURCE_FILES})
target_include_directories(io PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:${BSC_DESTINATION_INCLUDE}>)
target_precompile_headers(io PRIVATE
        )
install(DIRECTORY io DESTINATION ${BSC_DESTINATION_INCLUDE}
        FILES_MATCHING PATTERN "*.h")
target_link_libraries(io PUBLIC LibMagic::LibMagic core exiv2 parser)
install(TARGETS io COMPONENT io DESTINATION ${BSC_DESTINATION_LIB} EXPORT ${PROJECT_NAME}Targets)
if (${BSC_ENABLE_TESTS})
    add_subdirectory(test)
endif ()