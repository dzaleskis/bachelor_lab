#pragma once
#include <array>
#include <random>
#include <functional>
#include <chrono>

namespace random_utils {
    // NOTE: random device does NOT work correctly on win with MinGW

    static uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    static std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
    static std::mt19937 mte(ss);

    int get_random_int(int min = INT_MIN, int max = INT_MAX) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(mte);
    }

    unsigned int get_random_uint(unsigned int min = 0, unsigned int max = UINT_MAX) {
        std::uniform_int_distribution<unsigned int> dist(min, max);
        return dist(mte);
    }

    bool get_random_bool() {
        std::uniform_int_distribution<int> dist(0, 1);
        return dist(mte) == 1;
    }
}