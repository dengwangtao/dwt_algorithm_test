

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



# # 遍历每个测试文件
# foreach(test_src ${TEST_SOURCES})
#     # 获取文件名（不含扩展名）
#     get_filename_component(test_name ${test_src} NAME_WE)
    
#     # 构建一个唯一的 target 名
#     set(test_target ${CMAKE_PROJECT_NAME}_test_${test_name})

#     # 创建可执行文件
#     add_executable(${test_target} ${test_src})

#     # 链接测试依赖库
#     target_link_libraries(${test_target}
#         gtest
#         gtest_main
#         ${CMAKE_PROJECT_NAME}_src
#     )

#     # 注册该测试 target 到 CTest
#     add_test(NAME ${test_target} COMMAND ${test_target})
# endforeach()