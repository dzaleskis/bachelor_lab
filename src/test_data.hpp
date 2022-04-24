#pragma once

#include <vector>
#include <random>

namespace utils {
    std::vector<int> shuffled_int(int size, std::mt19937_64 &rng) {
        std::vector<int> v;
        v.reserve(size);
        for (int i = 0; i < size; ++i) v.push_back(i);
        std::shuffle(v.begin(), v.end(), rng);
        return v;
    }

    std::vector<int> shuffled_16_values_int(int size, std::mt19937_64 &rng) {
        std::vector<int> v;
        v.reserve(size);
        for (int i = 0; i < size; ++i) v.push_back(i % 16);
        std::shuffle(v.begin(), v.end(), rng);
        return v;
    }

    std::vector<int> all_equal_int(int size, std::mt19937_64 &rng) {
        std::vector<int> v;
        v.reserve(size);
        for (int i = 0; i < size; ++i) v.push_back(0);
        return v;
    }

    std::vector<int> ascending_int(int size, std::mt19937_64 &rng) {
        std::vector<int> v;
        v.reserve(size);
        for (int i = 0; i < size; ++i) v.push_back(i);
        return v;
    }

    std::vector<int> descending_int(int size, std::mt19937_64 &rng) {
        std::vector<int> v;
        v.reserve(size);
        for (int i = size - 1; i >= 0; --i) v.push_back(i);
        return v;
    }

    std::vector<int> pipe_organ_int(int size, std::mt19937_64 &rng) {
        std::vector<int> v;
        v.reserve(size);
        for (int i = 0; i < size / 2; ++i) v.push_back(i);
        for (int i = size / 2; i < size; ++i) v.push_back(size - i);
        return v;
    }

    std::vector<int> push_front_int(int size, std::mt19937_64 &rng) {
        std::vector<int> v;
        v.reserve(size);
        for (int i = 1; i < size; ++i) v.push_back(i);
        v.push_back(0);
        return v;
    }

    std::vector<int> push_middle_int(int size, std::mt19937_64 &rng) {
        std::vector<int> v;
        v.reserve(size);
        for (int i = 0; i < size; ++i) {
            if (i != size / 2) v.push_back(i);
        }
        v.push_back(size / 2);
        return v;
    }

    std::vector<int> unshift_back_int(int size, std::mt19937_64 &rng) {
        std::vector<int> v;
        v.reserve(size);
        v.push_back(size);
        for (int i = 1; i < size; ++i) v.push_back(i);
        return v;
    }

    // TODO: validate
    std::vector<int> partially_sorted_int(int size, std::mt19937_64 &rng) {
        std::vector<int> v;
        v.reserve(size);
        for (int i = 0; i < size; ++i) v.push_back(i);
        std::partial_sort(v.begin(), v.begin() + size / 2, v.end());
        return v;
    }

    // TODO: almost sorted (check FB messages)

//    std::vector<DistrF> all_test_data(int size, std::mt19937_64 &rng) {
//        return {
//                shuffled_int,
//                shuffled_16_values_int,
//                all_equal_int,
//                ascending_int,
//                descending_int,
//                pipe_organ_int,
//                push_front_int,
//                push_middle_int,
//                unshift_back_int,
//                partially_sorted_int,
//        };
//    }
//
//    std::vector<DistrF> random_data() {
//        return {
//                shuffled_int,
//                shuffled_16_values_int,
//        };
//    }

}
