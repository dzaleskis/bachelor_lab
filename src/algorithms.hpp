#pragma once
#include <array>

namespace algorithms {

    struct ops_stats {
        unsigned int assignments = 0;
        unsigned int comparisons = 0;
    };

    template <typename T>
    void shellsort_classic(std::vector<T>& vector, const std::vector<int>& gaps) {
        const std::size_t size = vector.size();

        for (int gap: gaps) {
            for (int i = gap; i < size; i++)
            {
                T temp = vector[i];
                int j = i;
                
                for (j; j >= gap && vector[j - gap] > temp; j -= gap)
                {
                    vector[j] = vector[j - gap];
                }

                vector[j] = temp;
            }
        }
    }

    template <typename T>
    ops_stats shellsort_classic_reported(std::vector<T>& vector, const std::vector<int>& gaps) {
        ops_stats stats;
        const std::size_t size = vector.size();

        for (int gap: gaps) {
            for (int i = gap; i < size; i++)
            {
                T temp = vector[i];
                stats.assignments += 1; // this is part of a swap, so needs to be counted
                int j = i;

                for (j; j >= gap && vector[j - gap] > temp; j -= gap)
                {
                    stats.comparisons += 1; // loop comparison vector[j - gap] > temp

                    vector[j] = vector[j - gap];
                    stats.assignments += 1;
                }
                if (j >= gap) { // this means vector[j - gap] > temp comparison was made
                    stats.comparisons += 1;
                }

                vector[j] = temp;
                stats.assignments += 1; // part of a swap
            }
        }

        return stats;
    }

    template <typename T>
    void shellsort_improved(std::vector<T>& vector, const std::vector<int>& gaps) {
        const std::size_t size = vector.size();

        for (int gap: gaps) {
            for (int i = gap; i < size; i++)
            {
                if (vector[i - gap] > vector[i]) {
                    T temp = vector[i];
                    int j = i;

                    do {
                        vector[j] = vector[j - gap];
                        j -= gap;

                    } while (j >= gap && vector[j - gap] > temp);

                    vector[j] = temp;
                }
            }
        }
    }

    template <typename T>
    ops_stats shellsort_improved_reported(std::vector<T>& vector, const std::vector<int>& gaps) {
        ops_stats stats;
        const std::size_t size = vector.size();

        for (int gap: gaps) {
            for (int i = gap; i < size; i++)
            {
                if (vector[i - gap] > vector[i]) {
                    T temp = vector[i];
                    stats.assignments += 1; // this is part of a swap, so needs to be counted
                    int j = i;
                    int loop_counter = 0;

                    do {
                        if (loop_counter > 0) {
                            stats.comparisons += 1; // vector[j - gap] > temp in the while condition
                        }

                        vector[j] = vector[j - gap];
                        j -= gap;

                        stats.assignments += 1; // vector[j] = vector[j - gap];
                        loop_counter++;
                    } while (j >= gap && vector[j - gap] > temp);

                    if (j >= gap) {
                        stats.comparisons += 1; // means vector[j - gap] > temp was executed, but false
                    }

                    vector[j] = temp;
                    stats.assignments += 1; // part of a swap
                }
                stats.comparisons += 1; // vector[i - gap] > vector[i] in the if statement
            }
        }

        return stats;
    }
}