#pragma once
#include <array>
#include <random>
#include <functional>
#include <algorithm>
#include <limits>
#include <iostream>

namespace array_utils {

    template <std::size_t SIZE>
    void fill_random(std::array<int, SIZE>& arr, int min = INT_MIN, int max = INT_MAX) {
        static std::random_device rd;
        static std::mt19937 mte(rd());

        std::uniform_int_distribution<int> dist(min, max);
        std::generate(arr.begin(), arr.end(), [&] () { return dist(mte); });
    }

    template <typename T, std::size_t SIZE>
    void fill_sorted_ascending(std::array<T, SIZE>& arr) {
        fill_random(arr);
        std::sort(arr.begin(), arr.end());
    }

    template <typename T, std::size_t SIZE>
    void fill_sorted_descending(std::array<T, SIZE>& arr) {
        fill_random(arr);
        std::sort(arr.begin(), arr.end(), std::greater<T>());
    }

}