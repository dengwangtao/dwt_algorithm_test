#pragma once

#include <unordered_map>
#include <functional>

#include "sort/bubble.h"
#include "sort/insert.h"
#include "sort/select.h"


namespace Sort
{



enum SortType
{
    SORT_NONE = 0,
    SORT_BUBBLE = 1,    // 冒泡排序
    SORT_INSERT = 2,    // 插入排序
    SORT_SELECT = 3,    // 选择排序
    SORT_QUICK = 4,     // 快速排序
    SORT_MERGE = 5,     // 归并排序
    SORT_HEAP = 6,      // 堆排序
    SORT_COUNT = 7,     // 计数排序
    SORT_BUCKET = 8,    // 桶排序
    SORT_RADIX = 9,     // 基数排序
    SORT_SHELL = 10,    // 希尔排序
    SORT_TIM = 11       // tim排序
};


// 使用 enable_if 技术进行排序算法的派发

#define SORT_DISPATCH(sort_type, sort_func) \
template <SortType type, typename T> \
typename std::enable_if<type == sort_type, void>::type \
sort(T* array, std::size_t length) \
{\
    Sort::sort_func(array, length); \
}


SORT_DISPATCH(SORT_BUBBLE, Bubble);

SORT_DISPATCH(SORT_INSERT, Insert);

SORT_DISPATCH(SORT_SELECT, Select);

}