
set(SOURCE_FILES parser/parameters/CommandLineParameters.cpp parser/parameters/CommandLineParameters.h parser/parser/fromString.h parser/cast/templateCast.h parser/parser/explode.h)

add_library(parser ${SOURCE_FILES})
target_include_directories(parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(parser PRIVATE ${LIBS})
install(TARGETS parser COMPONENT parser)

add_subdirectory(test)