#pragma once
#include <array>
#include <random>
#include <functional>

namespace random_utils {

    int get_random(int min = INT_MIN, int max = INT_MAX) {
        static std::random_device rd;
        static std::mt19937 mte(rd());

        std::uniform_int_distribution<int> dist(min, max);
        return dist(mte);
    }

    int get_random(unsigned int min = 0, unsigned int max = UINT_MAX) {
        static std::random_device rd;
        static std::mt19937 mte(rd());

        std::uniform_int_distribution<unsigned int> dist(min, max);
        return dist(mte);
    }

    double get_random(double min, double max) {
        static std::random_device rd;
        static std::mt19937 mte(rd());

        std::uniform_real_distribution<double> dist(min, max);
        return dist(mte);
    }
}