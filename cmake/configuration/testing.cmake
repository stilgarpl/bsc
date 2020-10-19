
option(BSC_ENABLE_TESTS "Enable unit tests" ON)
if (BSC_ENABLE_TESTS)
    enable_testing()
    find_package(Catch2 REQUIRED)
    include(Catch)
endif()