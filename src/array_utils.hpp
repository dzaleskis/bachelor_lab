#pragma once
#include <array>
#include <functional>
#include <algorithm>
#include "random_utils.hpp"

namespace array_utils {

    template <std::size_t SIZE>
    void fill_random(std::array<int, SIZE>& arr, int min = INT_MIN, int max = INT_MAX) {
        std::generate(arr.begin(), arr.end(), [&] () {
            return random_utils::get_random(min, max);
        });
    }

    template <typename T, std::size_t SIZE>
    void fill_sorted_ascending(std::array<T, SIZE>& arr, int min = INT_MIN, int max = INT_MAX) {
        fill_random(arr, min, max);
        std::sort(arr.begin(), arr.end());
    }

    template <typename T, std::size_t SIZE>
    void fill_sorted_descending(std::array<T, SIZE>& arr, int min = INT_MIN, int max = INT_MAX) {
        fill_random(arr, min, max);
        std::sort(arr.begin(), arr.end(), std::greater<T>());
    }

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