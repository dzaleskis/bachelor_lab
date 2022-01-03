#pragma once
#include <array>
#include <random>
#include <functional>
#include <chrono>

namespace utils {
    // shamelessly stolen from openGA
    static uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    static std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
    static std::mt19937 mte(ss);

    int get_random_int(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(mte);
    }

    unsigned int get_random_uint(unsigned int min, unsigned int max) {
        std::uniform_int_distribution<unsigned int> dist(min, max);
        return dist(mte);
    }

    bool get_random_bool() {
        std::uniform_int_distribution<int> dist(0, 1);
        return dist(mte) == 1;
    }
}