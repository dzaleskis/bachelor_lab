#pragma once
#include <cmath>
#include <chrono>
#include <random>
#include "element.hpp"
#include "sorting_algorithm.hpp"
#include "classic_algorithms.hpp"
#include "gaps.hpp"
#include "inversion_utils.hpp"
#include "cycles.hpp"
#include "test_data.hpp"
#include "custom_algorithms.hpp"

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


thread_local static std::random_device rand_dev;
thread_local static std::mt19937_64 rng(rand_dev());

static SortingStats get_sorting_stats(
        const std::function<void(std::vector<int>&)>& raw_sort_fn,
        const std::function<void(std::vector<Element<int>>&)>& element_sort_fn,
        int size) {
    auto all_data = utils::all_random_data(size, rng);

    double total_inversions = 0;
    double total_comparisons = 0;
    double total_assignments = 0;
    double total_cycles = 0;

    for (const auto& data: all_data) {
        std::vector<int> raw(data.begin(), data.end());
        auto cycles = measure_cycles([&]() {
            raw_sort_fn(raw);
        });

        std::vector<Element<int>> elements(data.begin(), data.end());
        auto report = global_measure.withReport([&]() {
            element_sort_fn(elements);
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

SortingStats get_genetic_sorting_stats(const AlgorithmBlueprint& algorithm, int size) {
    auto raw_sort_fn = [&](std::vector<int>& data) {
        run_algorithm(data, algorithm);
    };
    auto element_sort_fn = [&](std::vector<Element<int>>& data) {
        run_algorithm(data, algorithm);
    };

    return get_sorting_stats(raw_sort_fn, element_sort_fn, size);
}

SortingStats get_classic_sorting_stats(ClassicAlgorithm algorithm, int size) {
    auto raw_sort_fn = [&](std::vector<int>& data) {
        run_classic_sort(algorithm, data);
    };
    auto element_sort_fn = [&](std::vector<Element<int>>& data) {
        run_classic_sort(algorithm, data);
    };

    return get_sorting_stats(raw_sort_fn, element_sort_fn, size);
}

SortingStats get_custom_sorting_stats(CustomAlgorithm algorithm, int size) {
    auto raw_sort_fn = [&](std::vector<int>& data) {
        run_custom_sort(algorithm, data);
    };
    auto element_sort_fn = [&](std::vector<Element<int>>& data) {
        run_custom_sort(algorithm, data);
    };

    return get_sorting_stats(raw_sort_fn, element_sort_fn, size);
}