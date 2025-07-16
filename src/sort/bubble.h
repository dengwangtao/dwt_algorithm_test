#pragma once


namespace Sort
{

#define SORT_NAME Bubble

template<typename T>
void SORT_NAME(T* arr, std::size_t length)
{
    for (std::size_t i = 0; i < length - 1; ++ i)
    {
        for (std::size_t j = 0; j < length - 1 - i; ++ j)
        {
            if (arr[j] > arr[j + 1])
            {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

#undef SORT_NAME

}