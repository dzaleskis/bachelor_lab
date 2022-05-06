#pragma once

#include <ctime>
#include <vector>
#include <random>
#include <chrono>
#include "test_data.hpp"
#include "classic_algorithms.hpp"
#include "cycles.hpp"

struct Statistic {
    double median;
    double iqr;
    double outliers;
};

void bench_int() {
    auto seed = std::time(0);
    std::mt19937_64 el;

    typedef std::vector<int> (*DistrF)(int, std::mt19937_64&);
    typedef void (*SortF)(std::vector<int>&);

    std::pair<std::string, DistrF> distributions[] = {
            {"shuffled_int", utils::shuffled_int},
//            {"shuffled_16_values_int", utils::shuffled_16_values_int},
//            {"all_equal_int", utils::all_equal_int},
//            {"ascending_int", utils::ascending_int},
//            {"descending_int", utils::descending_int},
//            {"pipe_organ_int", utils::pipe_organ_int},
//            {"push_front_int", utils::push_front_int},
//            {"push_middle_int", utils::push_middle_int}
    };

    std::pair<std::string, SortF> sorts[] = {
            {"insertion_sort", &insertion_sort},
            {"shell_sort_ciura", &ciura_shell_sort},
            {"shell_sort_tokuda", &tokuda_shell_sort},
//            {"standard_sort", &standard_sort},
    };

    int sizes[] = {64, 256 };
    const int bench_ms = 5000;
    const int warmup_ms = bench_ms / 2;

    for (auto& distribution : distributions) {
        for (auto& sort: sorts) {
            el.seed(seed);

            for (auto size : sizes) {
                std::chrono::time_point<std::chrono::steady_clock> warmup_start = std::chrono::steady_clock::now();

                // perform warmup for `warmup_ms` to reduce chance of outliers
                while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - warmup_start).count() < warmup_ms) {
                    std::vector<int> v = distribution.second(size, el);

                    sort.second(v);
                }

                std::chrono::time_point<std::chrono::steady_clock> total_start = std::chrono::steady_clock::now();
                std::vector<uint64_t> cycles;

                // run the benchmark for `bench_ms`
                while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - total_start).count() < bench_ms) {
                    std::vector<int> v = distribution.second(size, el);

                    uint64_t start = rdtsc();
                    sort.second(v);
                    uint64_t end = rdtsc();

                    cycles.push_back(end - start);
                }

                std::sort(cycles.begin(), cycles.end());

                auto sample_size = cycles.size();
                auto quartile_size = sample_size / 4;

                auto min = cycles.front();
                auto max = cycles.back();
                auto median = cycles[sample_size / 2];

                // quarter of the way through the sample
                auto q1 = cycles[quartile_size];
                // three quarters of the way through the sample
                auto q3 = cycles[sample_size - quartile_size];
                auto iqr = q3 - q1;

                auto lower_outlier_range = q1 - (iqr * 1.5);
                auto upper_outlier_range = q3 + (iqr * 1.5);

                auto outlier_count = std::accumulate(cycles.begin(), cycles.end(), 0, [&](int acc, int c) {
                    if (c < lower_outlier_range || c > upper_outlier_range) {
                        return acc + 1;
                    }

                    return acc;
                });

                auto outliers = double(outlier_count) / sample_size;

                std::cerr << size << " " << distribution.first << " " << sort.first
                    << " " << median << " " << iqr << " " << outliers <<"\n";
            }
        }
    }
}

void bench_for_perf() {
    auto seed = std::time(0);
    std::mt19937_64 el(seed);

    std::chrono::time_point<std::chrono::steady_clock> total_start = std::chrono::steady_clock::now();

    while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - total_start).count() < 10000) {
        std::vector<int> v = utils::shuffled_int(64, el);

        // MARKER: replace when needed
        test_shell_sort(v);
    }
}