#pragma once


namespace Sort
{

#define SORT_NAME Insert

template<typename T>
void SORT_NAME(T* arr, std::size_t length)
{
    for (std::size_t i = 0; i < length; ++ i)
    {
        for (std::size_t j = i; j > 0 && arr[j] < arr[j - 1]; -- j)
        {
            std::swap(arr[j], arr[j - 1]);
        }
    }
}

#undef SORT_NAME

}