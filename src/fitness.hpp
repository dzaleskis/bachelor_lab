#pragma once

#include "iterator_utils.hpp"

namespace fitness {

    class SortingFitness {
        public:
            SortingFitness(double avg_inversions): avg_inversions(avg_inversions) {}

        private:
            double avg_inversions;
    };

    SortingFitness get_sorting_fitness(const std::vector<int>& gaps, int size, int runs) {
        std::vector<int> vec = std::vector<int>(size);
        double total_inversions = 0;

        for (int i = 0; i < runs; i++) {
            iterator_utils::fill_random(vec.begin(), vec.end());
            total_inversions += 1;
        }

        SortingFitness fitness(total_inversions / runs);

        return fitness;
    }
}