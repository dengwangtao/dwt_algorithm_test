#include <gtest/gtest.h>
#include <algorithm>
#include <random>

#include "sort/sort.h"


constexpr std::size_t TEST_ARR_LEN = 100;


static std::vector<int> MakeRandomVector(int size)
{
    std::vector<int> v;
    v.reserve(size);
    // 使用c++的随机数生成器
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);
    std::generate_n(std::back_inserter(v), size, [&](){ return dis(gen); });
    return v;
}

static std::vector<int> SortedVector(const std::vector<int>& v)
{
    std::vector<int> sorted(v);
    std::sort(sorted.begin(), sorted.end());
    return sorted;
}

// random vector
#define RV(v) std::vector<int> v = MakeRandomVector(TEST_ARR_LEN)
// sorted vector
#define SV(v) SortedVector(v)


TEST(SortTest, bubble)
{
    RV(v);
    Sort::sort<Sort::SORT_BUBBLE>(v.data(), v.size());
    EXPECT_EQ(v, SV(v));
}

TEST(SortTest, insert)
{
    RV(v);
    Sort::sort<Sort::SORT_INSERT>(v.data(), v.size());

    std::ostringstream oss;
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(oss, " "));
    GTEST_LOG_(INFO) << oss.str();

    EXPECT_EQ(v, SV(v));
}

TEST(SortTest, select)
{
    RV(v);
    Sort::sort<Sort::SORT_SELECT>(v.data(), v.size());
    EXPECT_EQ(v, SV(v));
}

TEST(SortTest, quick)
{
    RV(v);
    Sort::sort<Sort::SORT_QUICK>(v.data(), v.size());
    EXPECT_EQ(v, SV(v));
}
