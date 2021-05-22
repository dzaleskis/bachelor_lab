#pragma once
#include <array>
#include <functional>
#include <algorithm>
#include "random_utils.hpp"
#include "iterator_utils.hpp"

namespace array_utils {

    template <typename T, std::size_t SIZE>
    int count_inversions(std::array<T, SIZE>& arr)
    {
        int inv_count = 0;

        for (int i = 0; i < SIZE - 1; i++)
            for (int j = i + 1; j < SIZE; j++)
                if (arr[i] > arr[j])
                    inv_count++;
    
        return inv_count;
    }

}