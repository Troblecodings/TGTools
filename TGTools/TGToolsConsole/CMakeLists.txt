cmake_minimum_required (VERSION 3.8)

include_directories("../TGTools")

add_executable (TGToolsConsole "TGTools.cpp" )
target_link_libraries(TGToolsConsole PUBLIC TGTools)
