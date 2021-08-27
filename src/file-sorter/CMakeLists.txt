add_executable(file-sorter main/main_file_sorter.cpp properties/FileSorterMapperProperties.cpp properties/FileSorterMapperProperties.h properties/Actions.h properties/Factories.h)

target_link_libraries(file-sorter io properties)
install(TARGETS bsc-demo bsc-daemon bsc-control bsc-test COMPONENT bsc)

