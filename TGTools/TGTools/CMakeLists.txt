# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Include sub-projects.
add_library (TGTools STATIC "private/Texture.cpp" "public/Texture.hpp" "public/Result.hpp" "public/Util.hpp" "private/Font.cpp" "public/Font.hpp" "public/Actor.hpp" "private/Actor.cpp" "public/json.hpp" "private/Map.cpp" "public/Map.hpp" "private/Material.cpp" "public/Material.hpp" "private/Shader.cpp" "public/Shader.hpp")
