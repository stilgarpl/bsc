option(BSC_USE_CCACHE "Use ccache to speed up building this project" ON)

if(BSC_USE_CCACHE)
find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set(CMAKE_CUDA_COMPILER_LAUNCHER "${CCACHE_PROGRAM}") # CMake 3.9+
endif ()
endif ()

option(CMAKE_UNITY_BUILD "Enable unity builds" OFF)
option(BSC_USE_PCH "Use precompiled headers" OFF)