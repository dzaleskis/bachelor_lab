#pragma once

#include <vector>
#include <cmath>
#include <random>

namespace utils {
    std::vector<int> shuffled_int(int size, std::mt19937_64 &rng) {
        std::vector<int> v;
        v.reserve(size);
        for (int i = 0; i < size; ++i) v.push_back(i);
        std::shuffle(v.begin(), v.end(), rng);
        return v;
    }

    std::vector<int> shuffled_mod_sqrt_int(int size, std::mt19937_64 &rng) {
        std::vector<int> v;
        v.reserve(size);
        int mod = int(sqrt(size));
        for (int i = 0; i < size; ++i) v.push_back(i % mod);
        std::shuffle(v.begin(), v.end(), rng);
        return v;
    }

    std::vector<int> descending_int(int size, std::mt19937_64 &rng) {
        std::vector<int> v;
        v.reserve(size);
        for (int i = size - 1; i >= 0; --i) v.push_back(i);
        return v;
    }

    std::vector<int> partially_sorted_int(int size, std::mt19937_64 &rng) {
        std::vector<int> v;
        v.reserve(size);
        for (int i = 0; i < size; ++i) v.push_back(i);
        std::shuffle(v.begin(), v.end(), rng);
        std::partial_sort(v.begin(), v.begin() + size / 2, v.end());
        return v;
    }

    std::vector<int> merge_int(int size, std::mt19937_64 &rng) {
        std::vector<int> v;
        v.reserve(size);
        for (int i = 0; i < size/2; ++i) v.push_back(i);
        for (int i = 0; i < size/2; ++i) v.push_back(i);
        return v;
    }

    std::vector<int> push_min_int(int size, std::mt19937_64 &rng) {
        std::vector<int> v;
        v.reserve(size);
        for (int i = 1; i < size; ++i) v.push_back(i);
        v.push_back(0);
        return v;
    }
}
