# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("TGTools")
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions /EHa- /EHs-")

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Include sub-projects.
add_subdirectory ("TGTools")
add_subdirectory ("TGToolsConsole")
add_subdirectory ("googletest")
add_subdirectory ("TGToolsTest")
