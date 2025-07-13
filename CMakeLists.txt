cmake_minimum_required(VERSION 3.10.0)
project(algorithm VERSION 0.1.0 LANGUAGES C CXX)


enable_testing()



set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)





add_subdirectory(src)

# 添加 googletest
add_subdirectory(third_party/googletest)

add_subdirectory(test)

add_subdirectory(demo)
