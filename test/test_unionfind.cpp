#include <gtest/gtest.h>
#include "union_find/union_find.h"



TEST(UnionFindTest, Test_UF_1)
{
    UnionFind<int> uf;

    uf.Union(1, 2);
    uf.Union(2, 3);
    uf.Union(4, 5);

    EXPECT_EQ(uf.Find(1), uf.Find(2));
    EXPECT_EQ(uf.Find(1), uf.Find(3));

    EXPECT_EQ(uf.Find(4), uf.Find(5));
}


TEST(UnionFindTest, Test_UF_2)
{
    UnionFind<int> uf;

    uf.Union(1, 2);
    uf.Union(2, 3);
    uf.Union(4, 5);

    GTEST_LOG_(INFO) << "UnionFindTest.Test_UF_2: "
        << uf.Find(1) << " "
        << uf.Find(2) << " "
        << uf.Find(3) << " "
        << uf.Find(4) << " "
        << uf.Find(5) << std::endl;

    EXPECT_EQ(uf.Find(1), 1);
    EXPECT_EQ(uf.Find(2), 1);
    EXPECT_EQ(uf.Find(3), 1);
    EXPECT_EQ(uf.Find(4), 4);
    EXPECT_EQ(uf.Find(5), 4);
}

TEST(UnionFindTest, Test_UF_3)
{
    using ull = unsigned long long;
    UnionFind<ull, std::greater<ull>> uf;

    uf.Union(1, 2);
    uf.Union(2, 3);
    uf.Union(4, 5);
    uf.Union(10000000000000000000ull, 10000000000000000001ull);

    GTEST_LOG_(INFO) << "UnionFindTest.Test_UF_2: "
        << uf.Find(1) << " "
        << uf.Find(2) << " "
        << uf.Find(3) << " "
        << uf.Find(4) << " "
        << uf.Find(5) << " "
        << uf.Find(10000000000000000000ull) << " "
        << uf.Find(10000000000000000001ull)
        << std::endl;

    EXPECT_EQ(uf.Find(1), 3);
    EXPECT_EQ(uf.Find(2), 3);
    EXPECT_EQ(uf.Find(3), 3);
    EXPECT_EQ(uf.Find(4), 5);
    EXPECT_EQ(uf.Find(5), 5);
    EXPECT_EQ(uf.Find(10000000000000000000ull), 10000000000000000001ull);
    EXPECT_EQ(uf.Find(10000000000000000001ull), 10000000000000000001ull);

    uf.Union(10000000000000000000ull, 1);
    EXPECT_EQ(uf.Find(1), 10000000000000000001ull);
}


TEST(UnionFindTest, Test_UF_4)
{
    // 对比字符串的大小
    auto cmp = [](int a, int b)
    {
        return std::to_string(a) < std::to_string(b);
    };
    UnionFind<int, decltype(cmp)> uf(cmp);

    uf.Union(2, 21);
    uf.Union(111, 21);

    EXPECT_EQ(uf.Find(2), 111);
    EXPECT_EQ(uf.Find(21), 111);
}