#pragma once

#include "algorithms.hpp"
#include "iterator_utils.hpp"

namespace fitness {

    struct gap_seq_fitness {
        double avg_comparisons;
        double avg_assignments;
    };

    gap_seq_fitness eval_classic_fitness(const std::vector<int>& gaps, int array_size, int runs) {
        std::vector<int> vec = std::vector<int>(array_size);
        double total_comparisons = 0;
        double total_assignments = 0;

        for (int i = 0; i < runs; i++) {
            iterator_utils::fill_random(vec.begin(), vec.end());
            auto stats = algorithms::shellsort_classic_reported(vec, gaps);
            total_comparisons += stats.comparisons;
            total_assignments += stats.assignments;
        }

        return { total_comparisons / runs, total_assignments / runs };
    }
}