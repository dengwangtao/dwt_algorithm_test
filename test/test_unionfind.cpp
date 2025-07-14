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

    GTEST_LOG_(INFO) << "UnionFindTest.Test_UF_3: "
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


TEST(UnionFindTest, Test_UF_5)
{
    struct Node
    {
        int value1 = 0;
        int value2 = 0;
        int value3 = 0;
        bool operator<(const Node& other) const
        {
            return std::tie(value1, value2, value3) < std::tie(other.value1, other.value2, other.value3);
        }
        bool operator==(const Node& other) const
        {
            return std::tie(value1, value2, value3) == std::tie(other.value1, other.value2, other.value3);
        }
        bool operator!=(const Node& other) const
        {
            return !(*this == other);
        }
    };
    struct NodeHash
    {
        size_t operator()(const Node& node) const
        {
            return std::hash<int>()(node.value1) ^ std::hash<int>()(node.value2) ^ std::hash<int>()(node.value3);
        }
    };

    UnionFind<Node, std::less<Node>, NodeHash> uf;

    Node node1 = {
        .value1 = 1,
        .value2 = 2,
        .value3 = 2
    };

    Node node2 = {
        .value1 = 1,
        .value2 = 2,
        .value3 = 3
    };

    Node node3 = {
        .value1 = 1,
        .value2 = 2,
        .value3 = 4
    };

    Node node4 = {
            .value1 = 2,
            .value2 = -1,
            .value3 = 3
        };



    uf.Union(node1, node2);
    uf.Union(node1, node3);
    uf.Union(node2, node4);

    EXPECT_EQ(uf.Find(node1), node1);
    EXPECT_EQ(uf.Find(node2), node1);
    EXPECT_EQ(uf.Find(node3), node1);
    EXPECT_EQ(uf.Find(node4), node1);
}