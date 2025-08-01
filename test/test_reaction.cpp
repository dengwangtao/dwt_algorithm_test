#include <gtest/gtest.h>
#include <numeric>
#include <chrono>

#include "reactions/react.h"



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

    auto x1 = clc1.get();
    auto x = clc2.get();


    ASSERT_FLOAT_EQ(clc1.get(), 4.14);
    EXPECT_EQ(clc2.get(), "14.140000");

    a.value(10);
    GTEST_LOG_(INFO) << "a.update(10)";
    // GTEST_LOG_(INFO) << a;
    // GTEST_LOG_(INFO) << b;
    // GTEST_LOG_(INFO) << clc1;
    // GTEST_LOG_(INFO) << clc2;
    
    ASSERT_FLOAT_EQ(clc1.get(), 13.14);
    EXPECT_EQ(clc2.get(), "1013.140000");

    b.value(20);
    GTEST_LOG_(INFO) << "b.update(20)";
    // GTEST_LOG_(INFO) << a;
    // GTEST_LOG_(INFO) << b;
    // GTEST_LOG_(INFO) << clc1;
    // GTEST_LOG_(INFO) << clc2;

    ASSERT_FLOAT_EQ(clc1.get(), 30);
    EXPECT_EQ(clc2.get(), "1030.000000");
}



TEST(TestReaction, reaction_copy)
{
    auto a = reactions::var(1);
    auto b = reactions::var(3.14);

    auto clc1 = reactions::calc(
        [](auto p1, auto p2) { return p1 + p2; },
        a, b
    );

    auto clc2 = reactions::calc(
        [](auto p1, auto p2) { return std::to_string(p1) + std::to_string(p2); },
        a, clc1
    );

    EXPECT_EQ(clc2.get(), "14.140000");

    auto clc3 = clc2;
    EXPECT_EQ(clc3.get(), "14.140000");

    a.value(2);
    EXPECT_EQ(clc2.get(), "25.140000");
    EXPECT_EQ(clc3.get(), "25.140000");
}



TEST(TestReaction, reaction_move)
{
    auto a = reactions::var(1);
    auto b = reactions::var(3.14);

    auto clc1 = reactions::calc(
        [](auto p1, auto p2) { return p1 + p2; },
        a, b
    );

    auto clc2 = reactions::calc(
        [](auto p1, auto p2) { return std::to_string(p1) + std::to_string(p2); },
        a, clc1
    );

    EXPECT_EQ(clc2.get(), "14.140000");

    auto clc3 = std::move(clc2);

    EXPECT_FALSE(static_cast<bool>(clc2));

    try
    {
        EXPECT_EQ(clc2.get(), "");
    }
    catch (const std::exception& e)
    {
        EXPECT_STREQ(e.what(), "react is expired");
    }
    EXPECT_EQ(clc3.get(), "14.140000");

    a.value(2);
    EXPECT_EQ(clc3.get(), "25.140000");
}



struct ProcessedData {
    std::string info;
    int checksum;
};

TEST(ReactionTest, StressTest) {
    using namespace reactions;
    using namespace std::chrono;

    // Create var-data sources
    auto base1 = var(1);                // Integer source
    auto base2 = var(2.0);              // Double source
    auto base3 = var(true);             // Boolean source
    auto base4 = var(std::string{"3"}); // String source
    auto base5 = var(4);                // Integer source

    // Layer 1: Add integer and double
    auto layer1 = calc([](int a, double b) {
        return a + b;
    }, base1, base2);

    // Layer 2: Multiply or divide based on the flag
    auto layer2 = calc([](double val, bool flag) {
        return flag ? val * 2 : val / 2;
    }, layer1, base3);

    // Layer 3: Convert double value to a string
    auto layer3 = calc([](double val) {
        return "Value:" + std::to_string(val);
    }, layer2);

    // Layer 4: Append integer to string
    auto layer4 = calc([](const std::string &s, const std::string &s4) {
        return s + "_" + s4;
    }, layer3, base4);

    // Layer 5: Get the length of the string
    auto layer5 = calc([](const std::string &s) {
        return s.length();
    }, layer4);

    // Layer 6: Create a vector of double values
    auto layer6 = calc([](size_t len, int b5) {
        return std::vector<int>(len, b5);
    }, layer5, base5);

    // Layer 7: Sum all elements in the vector
    auto layer7 = calc([](const std::vector<int> &vec) {
        return std::accumulate(vec.begin(), vec.end(), 0);
    }, layer6);

    // Layer 8: Create a ProcessedData object with checksum and info
    auto layer8 = calc([](int sum) {
        return ProcessedData{"ProcessedData", static_cast<int>(sum)};
    }, layer7);

    // Layer 9: Combine info and checksum into a string
    auto layer9 = calc([](const ProcessedData &calc) {
        return calc.info + "|" + std::to_string(calc.checksum);
    }, layer8);

    // Final layer: Add "Final:" prefix to the result
    auto finalLayer = calc([](const std::string &s) {
        return "Final:" + s;
    }, layer9);

    const int ITERATIONS = 100000;
    auto start = steady_clock::now(); // Start measuring time

    // Perform stress test for the given number of iterations
    for (int i = 0; i < ITERATIONS; ++i) {
        // Update base sources with new values
        base1.value(i % 100);
        base2.value(static_cast<double>(i % 100) * 0.1);
        base3.value(i % 2 == 0);

        // Simplified computation for expected result
        double l1 = base1.get() + base2.get();                        // Add base1 and base2
        double l2 = base3.get() ? l1 * 2 : l1 / 2;                    // Multiply or divide based on base3
        std::string l3 = "Value:" + std::to_string(l2);               // Convert to string
        std::string l4 = l3 + "_" + base4.get();                      // Append base1
        size_t l5 = l4.length();                                      // Get string length
        std::vector<int> l6(l5, base5.get());                         // Create vector of length 'l5'
        int l7 = std::accumulate(l6.begin(), l6.end(), 0);            // Sum vector values
        ProcessedData l8{"ProcessedData", static_cast<int>(l7)};      // Create ProcessedData object
        std::string l9 = l8.info + "|" + std::to_string(l8.checksum); // Combine info and checksum
        std::string expected = "Final:" + l9;                          // Add final prefix

        // Print progress every 10,000 iterations
        if (i % 10000 == 0) {
            auto dur = duration_cast<milliseconds>(steady_clock::now() - start);
            GTEST_LOG_(INFO)
                << "Progress: " << i << "/" << ITERATIONS
                << " (" << dur.count() << "ms)"
                << " - Expected: " << (finalLayer.get() == expected);
        }

        // Compare final result and expected
        EXPECT_EQ(finalLayer.get(), expected);
    }

    // Output the final results of the stress test
    auto duration = duration_cast<milliseconds>(steady_clock::now() - start);
    GTEST_LOG_(INFO)
        << "\n=== Stress Test Results ===\n"
        << "Iterations: " << ITERATIONS << "\n"
        << "Total time: " << duration.count() << "ms\n"
        << "Avg time per update: "
        << duration.count() / static_cast<double>(ITERATIONS) << "ms";
}