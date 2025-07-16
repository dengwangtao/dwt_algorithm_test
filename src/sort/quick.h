#pragma once
#include <cstddef>

namespace Sort
{

#define SORT_NAME Quick

template<typename T>
void SORT_NAME(T* arr, std::size_t begin, std::size_t end)
{
    if (begin >= end) return;

    T pivot = arr[begin + (end - begin) / 2];
    std::size_t left = begin;
    std::size_t right = end;

    while (left <= right)
    {
        // 找到左边大于 pivot 的元素
        while (arr[left] < pivot)
        {
            ++left;
        }
        // 找到右边小于等于 pivot 的元素
        while (arr[right] > pivot)
        {
            --right;
        }

        // 防止指针交叉
        if (left <= right)
        {
            std::swap(arr[left], arr[right]);
            ++left;
            --right;
        }
    }
    // while 结束， right < left

    // 递归处理左右两部分
    if (begin < right)
    {
        SORT_NAME(arr, begin, right);
    }
    if (left < end)
    {
        SORT_NAME(arr, left, end);
    }
}


template<typename T>
void SORT_NAME(T* arr, std::size_t length)
{
    SORT_NAME(arr, 0, length - 1);
}

#undef SORT_NAME

}