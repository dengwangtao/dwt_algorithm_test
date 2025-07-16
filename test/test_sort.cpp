#include <gtest/gtest.h>

#include "sort/sort.h"


#define VV std::vector<int> v = { 5, 4, 100, 3, 2, 1 }
#define VV2 std::vector<int>({ 1, 2, 3, 4, 5, 100 })


TEST(SortTest, bubble)
{
    VV;
    Sort::sort<Sort::SORT_BUBBLE>(v.data(), v.size());
    EXPECT_EQ(v, VV2);
}

TEST(SortTest, insert)
{
    std::vector<int> v = { 5, 4, 100, 3, 2, 1 };
    Sort::sort<Sort::SORT_INSERT>(v.data(), v.size());

    std::ostringstream oss;
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(oss, " "));
    GTEST_LOG_(INFO) << oss.str();

    EXPECT_EQ(v, VV2);
}

