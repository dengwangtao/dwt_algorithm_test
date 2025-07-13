

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS *.cpp *.h)

set(TARGET ${CMAKE_PROJECT_NAME}_src)

add_library(${TARGET} STATIC ${SRC_FILES})

# 这行让其他目录能通过 `#include "hello/xxx.h"` 访问
target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})



