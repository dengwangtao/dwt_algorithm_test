#include <gtest/gtest.h>

#include "hello/hello.h"



TEST(HelloTest, hello)
{
    EXPECT_EQ(hello(), "Hello, world!");
}