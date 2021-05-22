#pragma once
#include <array>

namespace algorithms {

    struct ops_stats {
        unsigned int assignments = 0;
        unsigned int comparisons = 0;
    };

    template <typename T>
    void shellsort_classic(std::vector<T>& vector, const std::vector<int>& gaps) {
        for (int gap: gaps) {
            for (int i = gap; i < vector.size(); i++)
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

        for (int gap: gaps) {
            for (int i = gap; i < vector.size(); i++)
            {
                stats.comparisons += 1;

                T temp = vector[i];
                int j = i;
                stats.assignments += 2;
                
                for (j; j >= gap && vector[j - gap] > temp; j -= gap)
                {
                    vector[j] = vector[j - gap];
                    stats.comparisons += 2;
                    stats.assignments += 2;
                }

                if (j < gap) {
                    stats.comparisons += 1;
                } else {
                    stats.comparisons += 2;
                }

                vector[j] = temp;
                stats.assignments += 1;
            }
        }

        return stats;
    }

    template <typename T>
    void shellsort_improved(std::vector<T>& vector, const std::vector<int>& gaps) {
        for (int gap: gaps) {
            for (int i = gap; i < vector.size(); i++)
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

        for (int gap: gaps) {
            for (int i = gap; i < vector.size(); i++)
            {
                stats.comparisons += 1;

                if (vector[i - gap] > vector[i]) {
                    T temp = vector[i];
                    int j = i;
                    stats.assignments += 2;

                    int loop_counter = 0;

                    do {
                        if (loop_counter > 0) {
                            stats.comparisons += 2;
                        }

                        vector[j] = vector[j - gap];
                        j -= gap;
                        stats.assignments += 2;

                        loop_counter++;
                    } while (j >= gap && vector[j - gap] > temp);

                    if (j < gap) {
                        stats.comparisons += 1;
                    } else {
                        stats.comparisons += 2;
                    }

                    vector[j] = temp;
                    stats.assignments += 1;
                }
                stats.comparisons += 1;
            }
        }

        return stats;
    }
}