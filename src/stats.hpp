#pragma once
#include <cmath>
#include <random>
#include "element.hpp"
#include "sorting_algorithm.hpp"
#include "inversion_utils.hpp"
#include "cycles.hpp"
#include "test_data.hpp"

struct SortStats {
    double avg_inversions;
    double avg_comparisons;
    double avg_assignments;
    double avg_swap_distance;
    double avg_cycles;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SortStats, avg_inversions, avg_assignments, avg_comparisons, avg_cycles, avg_swap_distance)

thread_local static std::random_device rand_dev;
thread_local static std::mt19937_64 rng(rand_dev());

static SortStats sort_stats(
        const std::function<void(std::vector<int>&)>& sort_fn,
        const std::function<void(TracedVec<Element<int>>&)>& traced_sort_fn,
        int size,
        int runs) {
    double total_inversions = 0;
    double total_comparisons = 0;
    double total_assignments = 0;
    double total_distance = 0;
    double total_accesses = 0;
    double total_cycles = 0;

    for (int i = 0; i < runs; i++) {
        std::vector<int> data = utils::shuffled_int(size, rng);
        // it's a dirty hack, but better than dealing with std::transform
        std::vector<Element<int>> tmp(data.begin(), data.end());
        TracedVec<Element<int>> elements(tmp);

        auto report = global_measure.withReport([&]() {
            traced_sort_fn(elements);
        });

        auto cycles = measure_cycles([&]() {
            sort_fn(data);
        });

        total_inversions += utils::merge_sort_with_inversions(elements);
        total_comparisons += report.comparisons;
        total_assignments += report.assignments;
        total_distance += report.total_distance;
        total_accesses += report.total_accesses;
        total_cycles += cycles;
    }

    return {total_inversions / runs, total_comparisons / runs, total_assignments / runs, total_distance / (total_accesses / 2), total_cycles / runs };
}


SortStats blueprint_sort_stats(const AlgorithmBlueprint& algorithm, int size, int runs) {
    auto sort_fn = [&](std::vector<int>& data) {
        run_algorithm(data, algorithm);
    };

    auto traced_sort_fn = [&](TracedVec<Element<int>>& data) {
        run_algorithm(data, algorithm);
    };

    return sort_stats(sort_fn, traced_sort_fn, size, runs);
}
