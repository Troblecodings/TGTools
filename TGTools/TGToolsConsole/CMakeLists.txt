cmake_minimum_required (VERSION 3.8)

include_directories("../TGTools")
link_directories("../../lib")
add_executable (TGToolsConsole "TGTools.cpp" )
target_link_libraries(TGToolsConsole PUBLIC TGTools glslang)

