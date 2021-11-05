#pragma once

#include "iterator_utils.hpp"

namespace fitness {

    struct SortingFitness {
        double avg_inversions;
    };

    SortingFitness get_sorting_fitness(const std::vector<int>& gaps, int size, int runs) {
        std::vector<int> vec = std::vector<int>(size);
        int total_inversions = 0;

        for (int i = 0; i < runs; i++) {
            iterator_utils::fill_random(vec.begin(), vec.end());

            // TODO: add the sorting here

            total_inversions += iterator_utils::count_inversions(vec.begin(), vec.end());
        }

        return {((double) total_inversions) / runs};
    }
}