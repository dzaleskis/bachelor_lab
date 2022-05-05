#pragma once

#include <ctime>
#include <vector>
#include <random>
#include <chrono>
#include "test_data.hpp"
#include "classic_algorithms.hpp"
#include "cycles.hpp"

struct Statistic {
    std::string name;
    double median;
    double mean;
    double deviation;
};

void bench_int() {
    auto seed = std::time(0);
    std::mt19937_64 el;

    typedef std::vector<int> (*DistrF)(int, std::mt19937_64&);
    typedef void (*SortF)(std::vector<int>&);

    std::pair<std::string, DistrF> distributions[] = {
            {"shuffled_int", utils::shuffled_int},
            {"shuffled_16_values_int", utils::shuffled_16_values_int},
            {"all_equal_int", utils::all_equal_int},
            {"ascending_int", utils::ascending_int},
            {"descending_int", utils::descending_int},
            {"pipe_organ_int", utils::pipe_organ_int},
            {"push_front_int", utils::push_front_int},
            {"push_middle_int", utils::push_middle_int}
    };

    std::pair<std::string, SortF> sorts[] = {
            {"insertion_sort", &insertion_sort},
            {"shell_sort_ciura", &ciura_shell_sort},
    };

    int sizes[] = {64, 256, 1024};

    for (auto& distribution : distributions) {
        for (auto& sort: sorts) {
            el.seed(seed);

            for (auto size : sizes) {
                std::chrono::time_point<std::chrono::steady_clock> total_start = std::chrono::steady_clock::now();
                std::chrono::time_point<std::chrono::steady_clock> total_end = std::chrono::steady_clock::now();
                std::vector<uint64_t> cycles;

                while (std::chrono::duration_cast<std::chrono::milliseconds>(total_end - total_start).count() < 5000) {
                    std::vector<int> v = distribution.second(size, el);

                    uint64_t start = rdtsc();
                    sort.second(v);
                    uint64_t end = rdtsc();

                    // add 0.5 to round up
                    cycles.push_back(uint64_t(double(end - start) / size + 0.5));
                    total_end = std::chrono::steady_clock::now();
                }

                // find median
                std::sort(cycles.begin(), cycles.end());
                auto median = cycles[cycles.size()/2];

                // find sum & mean
                double sum = std::accumulate(cycles.begin(), cycles.end(), 0.0);
                double mean = sum / cycles.size();

                double accum = 0.0;
                std::for_each (std::begin(cycles), std::end(cycles), [&](const double d) {
                    accum += (d - mean) * (d - mean);
                });
                double stdev = sqrt(accum / (cycles.size()-1));

                std::cerr << size << " " << distribution.first << " " << sort.first
                          << " " << median << " " << mean << " " << stdev<<"\n";
            }
        }
    }
}

void bench_for_perf(int size) {
    auto seed = std::time(0);
    std::mt19937_64 el(seed);

    std::chrono::time_point<std::chrono::steady_clock> total_start = std::chrono::steady_clock::now();
    std::chrono::time_point<std::chrono::steady_clock> total_end = std::chrono::steady_clock::now();

    while (std::chrono::duration_cast<std::chrono::milliseconds>(total_end - total_start).count() < 10000) {
        std::vector<int> v = utils::shuffled_int(size, el);

        // MARKER: replace when needed
        ciura_shell_sort(v);

        total_end = std::chrono::steady_clock::now();
    }
}