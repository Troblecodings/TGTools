# CMakeList.txt : CMake project for TGTools, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's library.
add_library (TGTools STATIC "Texture.cpp" "Texture.hpp" "Result.hpp" "Util.hpp")

# TODO: Add tests and install targets if needed.
