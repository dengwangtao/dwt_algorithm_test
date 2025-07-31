#include <gtest/gtest.h>
#include "reactions/data_source.h"



TEST(TestReaction, reaction)
{
    auto a = reactions::var(1);
    auto b = reactions::var(3.14);

    EXPECT_EQ(a.get(), 1);
    EXPECT_EQ(b.get(), 3.14);

    auto clc1 = reactions::calc(
        [](auto p1, auto p2) { return p1 + p2; },
        a, b
    );

    auto clc2 = reactions::calc(
        [](auto p1, auto p2) { return std::to_string(p1) + std::to_string(p2); },
        a, clc1
    );

    auto x = clc2.get();


    ASSERT_FLOAT_EQ(clc1.get(), 4.14);
    EXPECT_EQ(clc2.get(), "14.140000");

    a.value(10);
    GTEST_LOG_(INFO) << "a.update(10)";
    GTEST_LOG_(INFO) << a;
    GTEST_LOG_(INFO) << b;
    GTEST_LOG_(INFO) << clc1;
    GTEST_LOG_(INFO) << clc2;
    
    ASSERT_FLOAT_EQ(clc1.get(), 13.14);
    EXPECT_EQ(clc2.get(), "1013.140000");
}