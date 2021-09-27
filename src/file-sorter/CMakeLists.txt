add_executable(file-sorter file-sorter/main/main_file_sorter.cpp file-sorter/properties/FileSorterMapperProperties.cpp file-sorter/properties/FileSorterMapperProperties.h file-sorter/properties/Actions.h file-sorter/properties/Factories.h file-sorter/properties/FileSorterPatternFactory.cpp file-sorter/properties/FileSorterPatternFactory.h)
target_include_directories(file-sorter PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:${BSC_DESTINATION_INCLUDE}>)

target_link_libraries(file-sorter io properties)
install(TARGETS bsc-demo bsc-daemon bsc-control bsc-test COMPONENT bsc)

