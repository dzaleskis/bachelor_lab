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

struct BenchmarkResult {
    int size;
    std::string algorithm;
    std::string distribution;
    Statistics comparisons;
    Statistics assignments;
    Statistics cycles;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(BenchmarkResult, size, algorithm, distribution, comparisons, assignments, cycles);

void bench_int() {
    auto seed = std::time(0);
    std::mt19937_64 el;

    typedef std::vector<int> (*DistrF)(int, std::mt19937_64&);
    typedef void (*SortF)(std::vector<int>&);
    typedef void (*ElSortF)(std::vector<Element<int>>&);

    std::pair<std::string, DistrF> distributions[] = {
            {"shuffled", utils::shuffled_int},
            {"shuffled_mod_sqrt", utils::shuffled_mod_sqrt_int},
            {"descending", utils::descending_int},
            {"partially_sorted", utils::partially_sorted_int},
            {"merge", utils::merge_int},
            {"push_min", utils::push_min_int},
    };

    std::tuple<std::string, SortF, ElSortF> small_sorts[] = {
            {"Ciura", &ciura_shell_sort, &ciura_shell_sort},
            {"Tokuda", &tokuda_shell_sort, &tokuda_shell_sort},
            {"Sedgewick", &sedgewick_shell_sort, &sedgewick_shell_sort},
            {"A1", &genetic_sort_a1, &genetic_sort_a1},
            {"A2", &genetic_sort_a2, &genetic_sort_a2},
            {"A3", &genetic_sort_a3, &genetic_sort_a3},
    };

    std::tuple<std::string, SortF, ElSortF> mid_sorts[] = {
            {"Ciura", &ciura_shell_sort, &ciura_shell_sort},
            {"Tokuda", &tokuda_shell_sort, &tokuda_shell_sort},
            {"Sedgewick", &sedgewick_shell_sort, &sedgewick_shell_sort},
            {"B1", &genetic_sort_b1, &genetic_sort_b1},
            {"B2", &genetic_sort_b2, &genetic_sort_b2},
            {"B3", &genetic_sort_b3, &genetic_sort_b3},
    };

    std::tuple<std::string, SortF, ElSortF> large_sorts[] = {
            {"Ciura", &ciura_shell_sort, &ciura_shell_sort},
            {"Tokuda", &tokuda_shell_sort, &tokuda_shell_sort},
            {"Sedgewick", &sedgewick_shell_sort, &sedgewick_shell_sort},
            {"C1", &genetic_sort_c1, &genetic_sort_c1},
            {"C2", &genetic_sort_c2, &genetic_sort_c2},
            {"C3", &genetic_sort_c3, &genetic_sort_c3},
    };

    const int max_size = 8192;
    const std::array<int, 4> sizes = { max_size >> 3, max_size >> 2, max_size >> 1, max_size };

    const int bench_ms = 10000;
    const int warmup_ms = bench_ms / 5;

    std::vector<BenchmarkResult> results;

    for (auto& distribution : distributions) {
        for (auto& sort: large_sorts) {
            el.seed(seed);

            for (auto size : sizes) {
                std::chrono::time_point<std::chrono::steady_clock> warmup_start = std::chrono::steady_clock::now();

                // perform warmup for `warmup_ms` to reduce chance of outliers
                while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - warmup_start).count() < warmup_ms) {
                    std::vector<int> v = distribution.second(size, el);

                    std::get<1>(sort)(v);
                }

                std::chrono::time_point<std::chrono::steady_clock> total_start = std::chrono::steady_clock::now();
                std::vector<uint64_t> comparisons;
                std::vector<uint64_t> assignments;
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

                BenchmarkResult result { size, std::get<0>(sort), distribution.first, cmp_stats, assign_stats, cycles_stats };

                results.push_back(result);

                std::cerr << size << " " << distribution.first << " " << std::get<0>(sort)
                    << " cycles: " << cycles_stats.median << " " << cycles_stats.iqr << " " << cycles_stats.outliers
                    << " comparisons: " << cmp_stats.median
                    << " assignments: " << assign_stats.median <<"\n";
            }
        }
    }

    std::ofstream output_file;
    output_file.open("bench_results_" + std::to_string(max_size));

    json resultJson(results);

    output_file << resultJson.dump(2) << std::endl;
    output_file.close();
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