
set(SOURCE_FILES parser/parameters/CommandLineParameters.cpp parser/parameters/CommandLineParameters.h parser/parser/FromString.h parser/cast/template_cast.h)

add_library(parser ${SOURCE_FILES})
target_include_directories(parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(parser PRIVATE ${LIBS})
install(TARGETS parser COMPONENT parser)

add_subdirectory(test)