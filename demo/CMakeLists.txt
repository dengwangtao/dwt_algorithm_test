

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS *.cpp *.h)

include_directories(${CMAKE_SOURCE_DIR}/src)

set(TARGET ${CMAKE_PROJECT_NAME}_demo)
add_executable(${TARGET} ${SRC_FILES})

target_link_libraries(${TARGET} ${CMAKE_PROJECT_NAME}_src)
