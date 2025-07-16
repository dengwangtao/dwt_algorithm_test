#pragma once
#include <cstddef>

namespace Sort
{

#define SORT_NAME Merge


template<typename T>
void SORT_NAME(T* arr, std::size_t begin, std::size_t end, T* temp)
{
    if (begin >= end)
    {
        return;
    }

    std::size_t mid = begin + (end - begin) / 2;
    SORT_NAME(arr, begin, mid, temp);
    SORT_NAME(arr, mid + 1, end, temp);

    std::size_t i = begin;
    std::size_t j = mid + 1;
    std::size_t k = 0;
    while (i <= mid && j <= end)
    {
        if (arr[i] < arr[j])
        {
            temp[begin + k] = arr[i];
            ++i;
        }
        else
        {
            temp[begin + k] = arr[j];
            ++j;
        }
        ++k;
    }
    while (i <= mid)
    {
        temp[begin + k] = arr[i];
        ++i;
        ++k;
    }
    while (j <= end)
    {
        temp[begin + k] = arr[j];
        ++j;
        ++k;
    }
    for (std::size_t i = begin; i <= end; ++i)
    {
        arr[i] = temp[i];
    }
}


template<typename T>
void SORT_NAME(T* arr, std::size_t length)
{
    T* temp = new T[length]; // 辅助空间
    SORT_NAME(arr, 0, length - 1, temp);
    delete[] temp;
}

#undef SORT_NAME

}