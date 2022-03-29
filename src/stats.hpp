#pragma once
#include <cmath>
#include <chrono>
#include "element.hpp"
#include "sorting_algorithm.hpp"
#include "classic_algorithms.hpp"
#include "gaps.hpp"
#include "inversion_utils.hpp"
#include "cycles.hpp"
#include "test_data.hpp"

struct SortingStats {
    double avg_inversions;
    double avg_comparisons;
    double avg_assignments;
    double avg_cycles;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SortingStats, avg_inversions, avg_assignments, avg_comparisons, avg_cycles)

uint64_t measure_cycles(const std::function<void()> & fn) {
    auto start = rdtsc();

    fn();

    auto end = rdtsc();
    auto duration = end - start;

    return duration;
}

// reuse the mersenne twister, since it is expensive to initialize
thread_local static std::mt19937_64 rng;

SortingStats get_sorting_stats(const AlgorithmBlueprint& algorithmBlueprint, int size) {
    rng.seed(std::time(0));
    auto all_data = utils::all_test_data(size, rng);

    double total_inversions = 0;
    double total_comparisons = 0;
    double total_assignments = 0;
    double total_cycles = 0;

    for (const auto& data: all_data) {
        std::vector<int> raw(data.begin(), data.end());
        auto cycles = measure_cycles([&]() {
            run_algorithm(raw, algorithmBlueprint);
        });

        std::vector<Element<int>> elements(data.begin(), data.end());
        auto report = global_measure.withReport([&]() {
            run_algorithm(elements, algorithmBlueprint);
        });

        total_cycles += cycles;
        total_inversions += utils::merge_sort_with_inversions(elements);
        total_comparisons += report.comparisons;
        total_assignments += report.assignments;
    }

    auto runs = all_data.size();
    auto stats = SortingStats { total_inversions / runs, total_comparisons / runs, total_assignments / runs, total_cycles / runs };

    return stats;
}

SortingStats get_classic_sorting_stats(ClassicAlgorithm algorithm, int size) {
    rng.seed(std::time(0));
    auto all_data = utils::all_test_data(size, rng);

    double total_inversions = 0;
    double total_comparisons = 0;
    double total_assignments = 0;
    double total_cycles = 0;

    for (const auto& data: all_data) {
        std::vector<int> raw(data.begin(), data.end());
        auto cycles = measure_cycles([&]() {
            run_classic_sort(algorithm, raw);
        });

        std::vector<Element<int>> elements(data.begin(), data.end());
        auto report = global_measure.withReport([&]() {
            run_classic_sort(algorithm, elements);
        });

        total_cycles += cycles;
        total_inversions += utils::merge_sort_with_inversions(elements);
        total_comparisons += report.comparisons;
        total_assignments += report.assignments;
    }

    auto runs = all_data.size();
    auto stats = SortingStats { total_inversions / runs, total_comparisons / runs, total_assignments / runs, total_cycles / runs };

    return stats;
}