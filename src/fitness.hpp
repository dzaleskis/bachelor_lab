#pragma once

#include "algorithms.hpp"
#include "array_utils.hpp"

namespace fitness {

    struct gap_seq_fitness {
        double avg_comparisons;
        double avg_assignments;
    };

    template <std::size_t A_SIZE, std::size_t G_SIZE>
    gap_seq_fitness eval_classic_fitness(const std::array<int, G_SIZE>& gaps, int runs) {
        std::array<int, A_SIZE> arr;
        double total_comparisons = 0;
        double total_assignments = 0;

        for (int i = 0; i < runs; i++) {
            array_utils::fill_random(arr);
            auto stats = algorithms::shellsort_classic_reported(arr, gaps);
            total_comparisons += stats.comparisons;
            total_assignments += stats.assignments;
        }

        return { total_comparisons / runs, total_assignments / runs };
    }

    template <std::size_t A_SIZE, std::size_t G_SIZE>
    gap_seq_fitness eval_improved_fitness(const std::array<int, G_SIZE>& gaps, int runs) {
        std::array<int, A_SIZE> arr;
        double total_comparisons = 0;
        double total_assignments = 0;

        for (int i = 0; i < runs; i++) {
            array_utils::fill_random(arr);
            auto stats = algorithms::shellsort_improved_reported(arr, gaps);
            total_comparisons += stats.comparisons;
            total_assignments += stats.assignments;
        }

        return { total_comparisons / runs, total_assignments / runs };
    }

}