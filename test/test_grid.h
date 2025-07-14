#include <gtest/gtest.h>
#include "grid/package_grid.h"

// Test for GridKey operations
TEST(GridTest, GridKeyTest)
{
    // 测试构造函数和操作符
    GridKey key1(10, 20);
    GridKey key2(10, 21);
    GridKey key3(10, 100);

    // 测试 `==` 运算符
    EXPECT_EQ(key1, key1); // 同一对象应该相等
    EXPECT_NE(key1, key2);  // 不同的对象应该不相等

    // 测试 `+` 和 `+=` 运算符
    GridKey key4 = key1 + key2;
    EXPECT_EQ(key4.Row(), 20);
    EXPECT_EQ(key4.Col(), 41); // Row = 10 + 10, Col = 20 + 21

    key1 += key2;
    EXPECT_EQ(key1.Row(), 20);
    EXPECT_EQ(key1.Col(), 41); // Row = 10 + 10, Col = 20 + 21

    // 测试比较运算符
    GridKey key5(10, 20);
    GridKey key6(10, 21);
    EXPECT_LT(key5, key6); // key5 应该小于 key6
    EXPECT_LE(key5, key6); // key5 应该小于等于 key6
    EXPECT_GT(key6, key5); // key6 应该大于 key5
    EXPECT_GE(key6, key5); // key6 应该大于等于 key5
}

// Test for GridRect validity and attributes
TEST(GridTest, GridRectTest)
{
    GridKey topLeft(0, 0);
    GridRect rect(topLeft, 2, 3);

    // 测试基本属性
    EXPECT_EQ(rect.TopLeft(), topLeft);  // TopLeft should be (0, 0)
    EXPECT_EQ(rect.Height(), 2);         // Height should be 2
    EXPECT_EQ(rect.Width(), 3);          // Width should be 3
    EXPECT_EQ(rect.BottomRight(), GridKey(1, 2));  // BottomRight should be (1, 2)
    EXPECT_TRUE(rect.IsValid());         // Rect should be valid

    // 测试无效 GridRect
    GridRect invalidRect(topLeft, -1, 3);
    EXPECT_FALSE(invalidRect.IsValid());  // Invalid rect with negative height
}

// Test for PackageGrid with for loop
TEST(GridTest, PackageGridForLoop)
{
    PackageGrid grid;

    // 设置一些数据
    grid.Set(GridKey(1, 1), 100);
    grid.Set(GridKey(2, 2), 200);
    grid.Set(GridKey(3, 3), 300);

    // 使用 for 循环遍历并验证数据
    for (auto it = grid.begin(); it != grid.end(); ++it)
    {
        const GridKey& key = it->first;
        u64 value = it->second;

        // 验证键值是否正确
        if (key.Row() == 1 && key.Col() == 1)
        {
            EXPECT_EQ(value, 100);
        }
        else if (key.Row() == 2 && key.Col() == 2)
        {
            EXPECT_EQ(value, 200);
        }
        else if (key.Row() == 3 && key.Col() == 3)
        {
            EXPECT_EQ(value, 300);
        }
        else
        {
            ADD_FAILURE() << "Unexpected key found: (" << key.Row() << ", " << key.Col() << ")";
        }
    }
}

// Test for PackageGrid basic operations
TEST(GridTest, PackageGridTest)
{
    PackageGrid grid;

    // 测试插入和访问
    GridKey key1(1, 1);
    grid.Set(key1, 100);
    EXPECT_EQ(grid.Get(key1), 100);

    // 测试更新值
    grid.Set(key1, 200);
    EXPECT_EQ(grid.Get(key1), 200);

    // 测试是否包含
    EXPECT_TRUE(grid.Contains(key1));

    // 测试删除
    grid.Remove(key1);
    EXPECT_EQ(grid.Get(key1), 0);
    EXPECT_FALSE(grid.Contains(key1));

    // 测试快照
    PackageGrid snapshot = grid.MakeSnapshot();
    EXPECT_EQ(snapshot.Get(key1), 0);  // 快照应该保留原有值

    // 测试矩阵操作
    GridRect rect(key1, 3, 3);
    grid.SetRect(rect, 50);  // 设置矩阵区域的值
    EXPECT_EQ(grid.Get(GridKey(1, 1)), 50);
    EXPECT_EQ(grid.Get(GridKey(2, 1)), 50);

    // 测试矩阵区域删除
    s32 rm_count = grid.RemoveRect(rect);
    EXPECT_EQ(rm_count, 9);  // 应该移除 3x3 区域的 9 个格子
}

// Test for PackageGrid rect operations
TEST(GridTest, PackageGridRect)
{
    std::vector<GridRect> rects;
    rects.emplace_back(GridKey(0 ,0), 3, 3);
    rects.emplace_back(GridKey(3, 1), 1, 1);
    rects.emplace_back(GridKey(3, 0), 3, 1);
    rects.emplace_back(GridKey(1, 3), 1, 1);
    rects.emplace_back(GridKey(5, 2), GridKey(6, 3));

    PackageGrid grid;

    int idx = 10000;
    for (auto& rect : rects)
    {
        bool is_free = grid.RectIsFree(rect);
        EXPECT_TRUE(is_free);
        grid.SetRect(rect, idx++);
    }

    // Check free rect space
    auto free_rect = grid.GetOneFreeRect(GridRect(GridKey(2,1), GridKey(6,3)), 2, 2);
    EXPECT_EQ(free_rect, GridRect(GridKey(3, 2), GridKey(4, 3)));
}

// Test for PackageGrid with fill operations
TEST(GridTest, FillPackageGridRect)
{
    PackageGrid grid;

    // 根据当前时间初始化随机种子
    srand(time(nullptr));

    std::vector<std::tuple<s32, s32, s32>> rects;
    for (s32 i = 1; i <= 4000; ++i)
    {
        int row = rand() % 5 + 1; // [1, 5]
        int col = rand() % 5 + 1; // [1, 5]
        rects.emplace_back(row, col, i * 10);
    }

    GridRect global_rect(GridKey(0, 0), GridKey(1000, 100));

    auto snapshot = grid.MakeSnapshot(); // 创建一个快照

    for (auto& rect : rects)
    {
        s32 height, width, value;
        std::tie(height, width, value) = rect;

        // 从快照中找到一个空闲区域
        auto free_rect = snapshot.GetOneFreeRect(global_rect, height, width);
        if (free_rect.IsValid())
        {
            snapshot.SetRect(free_rect, value);
        }
        else
        {
            std::cerr << "[ERROR] " << height << " " << width << " can't put " << global_rect << std::endl;
        }
    }

    std::cout << snapshot.DebugShapeString() << std::endl;
}

// Test for PackageGrid with fill operations with a random key
TEST(GridTest, FillPackageGridRect2)
{
    PackageGrid grid;

    // 根据当前时间初始化随机种子
    srand(time(nullptr));

    std::vector<std::tuple<s32, s32, s32>> rects;
    for (s32 i = 1; i <= 100; ++i)
    {
        int row = rand() % 5 + 1; // [1, 5]
        int col = rand() % 5 + 1; // [1, 5]
        rects.emplace_back(row, col, i * 10);
    }

    GridRect global_rect(GridKey(0, 0), GridKey(100, 100));

    auto snapshot = grid.MakeSnapshot(); // 创建一个快照

    GridKey random_key = GridKey(rand() % 100, rand() % 100);
    std::cout << "random key: " << random_key << std::endl;

    for (auto& rect : rects)
    {
        s32 height, width, value;
        std::tie(height, width, value) = rect;

        // 从快照中找到一个空闲区域
        auto free_rect = snapshot.GetNearestFreeRect(global_rect, random_key, height, width);
        if (free_rect.IsValid())
        {
            snapshot.SetRect(free_rect, value);
            std::cout << "[INFO] " << height << " " << width << " put to " << free_rect << std::endl;
        }
        else
        {
            std::cout << "[ERROR] " << height << " " << width << " can't put " << global_rect << std::endl;
        }
    }
}

   
