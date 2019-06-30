find_package(spdlog CONFIG REQUIRED)

set(SOURCE_FILES)


add_library(logic SHARED ${SOURCE_FILES})
target_include_directories(logic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} PRIVATE SYSTEM spdlog::spdlog)
target_link_libraries(logic ${LIBS} spdlog::spdlog)
install(TARGETS logic COMPONENT logic)

add_subdirectory(test)