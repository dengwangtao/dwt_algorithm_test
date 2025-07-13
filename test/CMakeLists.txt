

include_directories(${CMAKE_SOURCE_DIR}/src)


aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TEST_SOURCES)


set(TARGET ${CMAKE_PROJECT_NAME}_tests)
# 构建一个测试可执行文件，可执行文件名为项目路径名
add_executable(${TARGET} ${TEST_SOURCES})


# 链接被测试模块和 gtest
target_link_libraries(${TARGET}
    gtest
    gtest_main
    ${CMAKE_PROJECT_NAME}_src  # 这是 src 目录下你定义的库
)

# 注册测试
add_test(NAME ${TARGET} COMMAND ${TARGET})