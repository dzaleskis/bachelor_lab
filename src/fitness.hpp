#pragma once
#include <cmath>
#include "iterator_utils.hpp"
#include "sorting_algorithm.hpp"

namespace fitness {

    struct SortingStats {
        int runs;
        double inversions;
        double comparisons;
        double assignments;
    };

    struct AvgSortingStats {
        explicit AvgSortingStats(const SortingStats & stats) {
            avg_inversions = stats.inversions / stats.runs;
            avg_comparisons = stats.comparisons / stats.runs;
            avg_assignments = stats.assignments / stats.runs;
        }

        double avg_inversions;
        double avg_comparisons;
        double avg_assignments;
    };

    SortingStats get_sorting_stats(const AlgorithmBlueprint& algorithmBlueprint, int size, int runs = 1) {
        auto elements = std::vector<Element<int>>(size);
        auto algorithm = ConcreteAlgorithmFactory::getConcreteAlgorithm<typeof elements>(algorithmBlueprint);

        double total_inversions = 0;
        double total_comparisons = 0;
        double total_assignments = 0;

        for (int i = 0; i < runs; i++) {
            iterator_utils::fill_random(elements.begin(), elements.end());

            auto report = global_measure.withReport([&]() {
                algorithm->sort(elements);
            });

            total_inversions += iterator_utils::count_inversions(elements.begin(), elements.end());
            total_comparisons += report.comparisons;
            total_assignments += report.assignments;
        }

        auto stats = SortingStats { runs, total_inversions, total_comparisons, total_assignments };

        return stats;
    }

    // lower is better
    double calculate_fitness(const SortingStats& sortingStats) {
        auto avgStats = AvgSortingStats(sortingStats);
        auto inversions = avgStats.avg_inversions;
        auto comparisons = avgStats.avg_comparisons;
        auto assignments = avgStats.avg_assignments;

        auto efficiency = std::pow(inversions, 2.0) + ((comparisons / assignments) * (comparisons + assignments));

        return efficiency;
    }
}