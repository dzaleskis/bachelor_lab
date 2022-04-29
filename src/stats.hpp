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

struct SortingStats {
    double avg_inversions;
    double avg_comparisons;
    double avg_assignments;
    double avg_cycles;
    double avg_swap_distance;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SortingStats, avg_inversions, avg_assignments, avg_comparisons, avg_cycles, avg_swap_distance)

thread_local static std::random_device rand_dev;
thread_local static std::mt19937_64 rng;

typedef std::vector<int> (*DistrF)(int, std::mt19937_64&);

static std::vector<DistrF> all_dists {
    utils::shuffled_int,
    utils::shuffled_16_values_int,
    utils::all_equal_int,
    utils::ascending_int,
    utils::descending_int,
    utils::pipe_organ_int,
    utils::push_front_int,
    utils::push_middle_int,
    utils::unshift_back_int,
    utils::partially_sorted_int,
};

static std::vector<DistrF> random_dists {
        utils::shuffled_int,
        utils::shuffled_16_values_int,
};

uint64_t measure_cycles(const std::function<void()> & fn) {
    auto start = rdtsc();

    fn();

    auto end = rdtsc();
    auto duration = end - start;

    return duration;
}

static SortingStats get_sorting_stats(
        const std::vector<DistrF>& distributions,
        const std::function<void(std::vector<int>&)>& raw_sort_fn,
        const std::function<void(TracedVec<Element<int>>&)>& element_sort_fn,
        int size,
        int runs) {
    double total_inversions = 0;
    double total_comparisons = 0;
    double total_assignments = 0;
    double total_cycles = 0;
    double total_distance = 0;
    double total_accesses = 0;

    // seed generator with random value
    rng.seed(rand_dev());

    for (const auto& dist: distributions) {
        for (int i = 0; i < runs; i++) {
            std::vector<int> data = dist(size, rng);
            std::vector<int> raw(data);
            std::vector<Element<int>> tmp(data.begin(), data.end());
            TracedVec<Element<int>> elements(tmp);

            auto cycles = measure_cycles([&]() {
                raw_sort_fn(raw);
            });

            auto report = global_measure.withReport([&]() {
                element_sort_fn(elements);
            });

            total_cycles += cycles;
            total_inversions += utils::merge_sort_with_inversions(elements);
            total_comparisons += report.comparisons;
            total_assignments += report.assignments;
            total_distance += report.total_distance;
            total_accesses += report.total_accesses;
        }
    }

    auto allRuns = runs * distributions.size();

    return { total_inversions / allRuns, total_comparisons / allRuns,total_assignments / allRuns, total_cycles / allRuns, total_distance / (total_accesses / 2) };
}

SortingStats get_genetic_sorting_stats(const AlgorithmBlueprint& algorithm, int size, int runs) {
    auto raw_sort_fn = [&](std::vector<int>& data) {
        run_algorithm(data, algorithm);
    };
    auto element_sort_fn = [&](TracedVec<Element<int>>& data) {
        run_algorithm(data, algorithm);
    };

    return get_sorting_stats(random_dists, raw_sort_fn, element_sort_fn, size, runs);
}

SortingStats get_classic_sorting_stats(ClassicAlgorithm algorithm, int size, int runs) {
    auto raw_sort_fn = [&](std::vector<int>& data) {
        run_classic_sort(algorithm, data);
    };
    auto element_sort_fn = [&](TracedVec<Element<int>>& data) {
        run_classic_sort(algorithm, data);
    };

    return get_sorting_stats(random_dists, raw_sort_fn, element_sort_fn, size, runs);
}
