#pragma once
#include <cmath>
#include <random>
#include "element.hpp"
#include "sorting_algorithm.hpp"
#include "inversion_utils.hpp"
#include "cycles.hpp"
#include "test_data.hpp"
#include "statistics.hpp"

struct SortStats {
    double avg_inversions;
    double avg_comparisons;
    double avg_assignments;
    double avg_cycles;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SortStats, avg_inversions, avg_comparisons, avg_assignments, avg_cycles)

thread_local static std::random_device rand_dev;
thread_local static std::mt19937_64 rng(rand_dev());

static SortStats sort_stats(
        const std::function<void(std::vector<int>&)>& sort_fn,
        const std::function<void(std::vector<Element<int>>&)>& traced_sort_fn,
        int size,
        int runs) {
    std::vector<int> inversions;
    std::vector<int> comparisons;
    std::vector<int> assignments;
    std::vector<int> cycles;

    for (int i = 0; i < runs; i++) {
        std::vector<int> data = utils::shuffled_int(size, rng);
        std::vector<Element<int>> elements(data.begin(), data.end());

        auto report = global_measure.withReport([&]() {
            traced_sort_fn(elements);
        });

        auto c = measure_cycles([&]() {
            sort_fn(data);
        });

        inversions.push_back(utils::merge_sort_with_inversions(elements));
        comparisons.push_back(report.comparisons);
        assignments.push_back(report.assignments);
        comparisons.push_back(report.comparisons);
        cycles.push_back(c);
    }

    // better to average, since it will cause larger effect
    double avg_inversions = calculate_average(inversions);
    // better to use median, since we want to get the most "average" behavior
    double median_comparisons = calculate_median(comparisons);
    double median_assignments = calculate_median(assignments);
    double median_cycles = calculate_median(cycles);

    return { avg_inversions, median_comparisons, median_assignments, median_cycles };
}


SortStats blueprint_sort_stats(const AlgorithmBlueprint& algorithm, int size, int runs) {
    auto sort_fn = [&](std::vector<int>& data) {
        run_algorithm(data, algorithm);
    };

    auto traced_sort_fn = [&](std::vector<Element<int>>& data) {
        run_algorithm(data, algorithm);
    };

    return sort_stats(sort_fn, traced_sort_fn, size, runs);
}
