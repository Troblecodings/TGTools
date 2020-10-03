cmake_minimum_required (VERSION 3.8)

include_directories("../TGTools")
include_directories("../googletest/googletest/include")

add_executable (TGToolsTest "TGToolsTest.cpp" )
target_link_libraries(TGToolsTest PUBLIC TGTools gtest_main)
add_test(NAME TGToolsTest COMMAND TGToolsTest)