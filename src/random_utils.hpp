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

    double get_random(double min, double max) {
        static std::random_device rd;
        static std::mt19937 mte(rd());

        std::uniform_real_distribution<double> dist(min, max);
        return dist(mte);
    }
}