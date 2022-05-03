#pragma once
#include <cmath>
#include <chrono>
#include <random>
#include <algorithm>
#include "element.hpp"
#include "sorting_algorithm.hpp"
#include "classic_algorithms.hpp"
#include "gaps.hpp"
#include "inversion_utils.hpp"
#include "cycles.hpp"
#include "test_data.hpp"

struct SortTrace {
    double avg_inversions;
    double avg_comparisons;
    double avg_assignments;
    double avg_swap_distance;
};

struct SortStats: SortTrace {
    SortStats() = default;

    SortStats(const SortTrace& ops, double cycles): SortTrace(ops) {
        avg_cycles = cycles;
    }

    double avg_cycles;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SortStats, avg_inversions, avg_assignments, avg_comparisons, avg_cycles, avg_swap_distance)

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

// precondition - rng is seeded before
static SortTrace benchmark_traced(
        const std::vector<DistrF>& distributions,
        const std::function<void(TracedVec<Element<int>>&)>& sort_fn,
        int size,
        int runs) {
    double total_inversions = 0;
    double total_comparisons = 0;
    double total_assignments = 0;
    double total_distance = 0;
    double total_accesses = 0;

    for (const auto& dist: distributions) {
        for (int i = 0; i < runs; i++) {
            std::vector<int> data = dist(size, rng);
            // it's a dirty hack, but better than dealing with std::transform
            std::vector<Element<int>> tmp(data.begin(), data.end());
            TracedVec<Element<int>> elements(tmp);

            auto report = global_measure.withReport([&]() {
                sort_fn(elements);
            });

            total_inversions += utils::merge_sort_with_inversions(elements);
            total_comparisons += report.comparisons;
            total_assignments += report.assignments;
            total_distance += report.total_distance;
            total_accesses += report.total_accesses;
        }
    }

    auto all_runs = runs * distributions.size();

    return {total_inversions / all_runs, total_comparisons / all_runs, total_assignments / all_runs, total_distance / (total_accesses / 2) };
}


// precondition - rng is seeded before
double benchmark_cycles(
        const std::vector<DistrF>& distributions,
        const std::function<void(std::vector<int>&)>& sort_fn,
        int size,
        int runs) {
    double total_cycles = 0;

    for (const auto& dist: distributions) {
        for (int i = 0; i < runs; i++) {
            std::vector<int> data = dist(size, rng);

            auto cycles = measure_cycles([&]() {
                sort_fn(data);
            });

            total_cycles += cycles;
        }
    }

    auto all_runs = runs * distributions.size();

    return total_cycles / all_runs;
}

SortTrace bench_traced_blueprint(const AlgorithmBlueprint& algorithm, int size, int runs) {
    auto sort_fn = [&](TracedVec<Element<int>>& data) {
        run_algorithm(data, algorithm);
    };

    return benchmark_traced(random_dists, sort_fn, size, runs);
}

double bench_cycles_blueprint(const AlgorithmBlueprint& algorithm, int size, int runs) {
    auto sort_fn = [&](std::vector<int>& data) {
        run_algorithm(data, algorithm);
    };

    return benchmark_cycles(random_dists, sort_fn, size, runs);
}

SortTrace bench_traced_classic(ClassicAlgorithm algorithm, int size, int runs) {
    auto sort_fn = [&](TracedVec<Element<int>>& data) {
        run_classic_sort(algorithm, data);
    };

    return benchmark_traced(random_dists, sort_fn, size, runs);
}

double bench_cycles_classic(ClassicAlgorithm algorithm, int size, int runs) {
    auto sort_fn = [&](std::vector<int>& data) {
        run_classic_sort(algorithm, data);
    };

    return benchmark_cycles(random_dists, sort_fn, size, runs);
}

SortStats blueprint_sort_stats(const AlgorithmBlueprint& algorithm, int size, int runs) {
    rng.seed(rand_dev());

    return SortStats(bench_traced_blueprint(algorithm, size, runs), bench_cycles_blueprint(algorithm, size, runs));
}

SortStats classic_sort_stats(ClassicAlgorithm algorithm, int size, int runs) {
    rng.seed(rand_dev());

    return SortStats(bench_traced_classic(algorithm, size, runs),bench_cycles_classic(algorithm, size, runs));
}
