#pragma once
#include <cmath>
#include <chrono>
#include "element.hpp"
#include "iterator_utils.hpp"
#include "sorting_algorithm.hpp"
#include "classic_algorithms.hpp"
#include "gaps.hpp"

typedef long long time_size;

struct SortingStats {
    double avg_inversions;
    double avg_comparisons;
    double avg_assignments;
    double avg_time;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SortingStats, avg_inversions, avg_assignments, avg_comparisons, avg_time)

time_size measure_time_us(const std::function<void()> & fn) {
    auto start = std::chrono::high_resolution_clock::now();

    fn();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    return duration.count();
}

SortingStats get_blueprint_sorting_stats(const AlgorithmBlueprint& algorithmBlueprint, int size, int runs) {
    auto raw = std::vector<int>(size);
    auto elements = std::vector<Element<int>>(size);

    auto rawAlgorithm = ConcreteAlgorithmFactory::getConcreteAlgorithm<std::vector<int>>(algorithmBlueprint);
    auto elementAlgorithm = ConcreteAlgorithmFactory::getConcreteAlgorithm<std::vector<Element<int>>>(algorithmBlueprint);

    double total_inversions = 0;
    double total_comparisons = 0;
    double total_assignments = 0;
    double total_time = 0;

    for (int i = 0; i < runs; i++) {
        utils::fill_random(raw.begin(), raw.end());

        auto time = measure_time_us([&]() {
            rawAlgorithm->sort(raw);
        });

        total_time += time;
    }

    for (int i = 0; i < runs; i++) {
        utils::fill_random(elements.begin(), elements.end());

        auto report = global_measure.withReport([&]() {
            elementAlgorithm->sort(elements);
        });

        total_inversions += utils::count_inversions(elements.begin(), elements.end());
        total_comparisons += report.comparisons;
        total_assignments += report.assignments;
    }

    auto stats = SortingStats { total_inversions / runs, total_comparisons / runs, total_assignments / runs, total_time / runs };

    return stats;
}

SortingStats get_classic_sorting_stats(ClassicAlgorithm algorithm, int size, int runs) {
    auto raw = std::vector<int>(size);
    auto elements = std::vector<Element<int>>(size);

    double total_inversions = 0;
    double total_comparisons = 0;
    double total_assignments = 0;
    double total_time = 0;

    for (int i = 0; i < runs; i++) {
        utils::fill_random(raw.begin(), raw.end());

        auto time = measure_time_us([&]() {
            perform_classic_sort(algorithm, raw);
        });

        total_time += time;
    }

    for (int i = 0; i < runs; i++) {
        utils::fill_random(elements.begin(), elements.end());

        auto report = global_measure.withReport([&]() {
            perform_classic_sort(algorithm, elements);
        });

        total_inversions += utils::count_inversions(elements.begin(), elements.end());
        total_comparisons += report.comparisons;
        total_assignments += report.assignments;
    }

    auto stats = SortingStats { total_inversions / runs, total_comparisons / runs, total_assignments / runs, total_time / runs };

    return stats;
}

// lower is better
double evaluate_fitness(const SortingStats& sortingStats) {
    auto inversions = sortingStats.avg_inversions;
    auto comparisons = sortingStats.avg_comparisons;
    auto assignments = sortingStats.avg_assignments;
    // TODO: how to scale time?
    auto time = sortingStats.avg_time;

    auto fitness = std::pow(inversions, 2.0) + std::pow(time, 1.5) + comparisons + assignments;

    return fitness;
}