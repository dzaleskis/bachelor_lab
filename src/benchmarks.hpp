#pragma once

#include <ctime>
#include <vector>
#include <random>
#include <chrono>
#include "test_data.hpp"
#include "algorithms.hpp"
#include "cycles.hpp"
#include "statistics.hpp"
#include "element.hpp"

void bench_int() {
    auto seed = std::time(0);
    std::mt19937_64 el;

    typedef std::vector<int> (*DistrF)(int, std::mt19937_64&);
    typedef void (*SortF)(std::vector<int>&);
    typedef void (*ElSortF)(std::vector<Element<int>>&);

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

    std::tuple<std::string, SortF, ElSortF> sorts[] = {
            {"shell_sort_ciura", &ciura_shell_sort, &ciura_shell_sort},
            {"shell_sort_tokuda", &tokuda_shell_sort, &tokuda_shell_sort},
//            {"standard_sort", &standard_sort},
    };

    int sizes[] = {64, 256 };
    const int bench_ms = 5000;
    const int warmup_ms = 1000;

    for (auto& distribution : distributions) {
        for (auto& sort: sorts) {
            el.seed(seed);

            for (auto size : sizes) {
                std::chrono::time_point<std::chrono::steady_clock> warmup_start = std::chrono::steady_clock::now();

                // perform warmup for `warmup_ms` to reduce chance of outliers
                while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - warmup_start).count() < warmup_ms) {
                    std::vector<int> v = distribution.second(size, el);

                    std::get<1>(sort)(v);
                }

                std::chrono::time_point<std::chrono::steady_clock> total_start = std::chrono::steady_clock::now();
                std::vector<int> comparisons;
                std::vector<int> assignments;
                std::vector<uint64_t> cycles;

                // run the benchmark for `bench_ms`
                while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - total_start).count() < bench_ms) {
                    std::vector<int> v = distribution.second(size, el);
                    std::vector<Element<int>> e(v.begin(), v.end());

                    auto c = measure_cycles([&]() {
                        std::get<1>(sort)(v);
                    });

                    auto report = global_measure.withReport([&]() {
                        std::get<2>(sort)(e);
                    });

                    comparisons.push_back(report.comparisons);
                    assignments.push_back(report.assignments);
                    cycles.push_back(c);
                }

                auto cmp_stats = calculate_statistics(comparisons);
                auto assign_stats = calculate_statistics(assignments);
                auto cycles_stats = calculate_statistics(cycles);

                std::cerr << size << " " << distribution.first << " " << std::get<0>(sort)
                    << " cycles: " << cycles_stats.median << " " << cycles_stats.iqr << " " << cycles_stats.outliers
                    << " comparisons: " << cmp_stats.median << " " << cmp_stats.iqr << " " << cmp_stats.outliers
                    << " assignments: " << assign_stats.median << " " << assign_stats.iqr << " " << assign_stats.outliers <<"\n";
            }
        }
    }
}

void bench_for_perf() {
    auto seed = std::time(0);
    std::mt19937_64 el(seed);
    int max_iter = 5000000;
    std::vector<std::vector<int>> all_data;
    all_data.reserve(max_iter);

    for (int i = 0; i < max_iter; i++) {
        all_data.emplace_back(utils::shuffled_int(64, el));
    }

    for (int i = 0; i < max_iter; i++) {
        auto& v = all_data[i];

        // MARKER: replace when needed
        insertion_sort(v);
    }
}