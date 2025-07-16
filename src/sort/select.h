#pragma once
#include <cstddef>

namespace Sort
{

#define SORT_NAME Select

template<typename T>
void SORT_NAME(T* arr, std::size_t length)
{
    for (std::size_t i = 0; i < length; ++ i)
    {
        std::size_t min_idx = i;
        for (std::size_t j = i + 1; j < length; ++ j)
        {
            if (arr[j] < arr[min_idx])
            {
                min_idx = j;
            }
        }
        std::swap(arr[i], arr[min_idx]);
    }
}

#undef SORT_NAME

}