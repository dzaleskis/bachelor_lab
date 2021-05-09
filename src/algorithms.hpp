#pragma once
#include <array>

namespace algorithms {

    struct ops_stats {
        unsigned int assignments = 0;
        unsigned int comparisons = 0;
    };

    template <typename T, std::size_t A_SIZE, std::size_t G_SIZE>
    void shellsort_classic(std::array<T, A_SIZE>& arr, const std::array<int, G_SIZE>& gaps) {
        for (int gap: gaps) {
            for (int i = gap; i < A_SIZE; i++)
            {
                T temp = arr[i];
                int j = i;
                
                for (j; j >= gap && arr[j - gap] > temp; j -= gap)
                {
                    arr[j] = arr[j - gap];
                }

                arr[j] = temp;
            }
        }
    }

    template <typename T, std::size_t A_SIZE, std::size_t G_SIZE>
    ops_stats shellsort_classic_reported(std::array<T, A_SIZE>& arr, const std::array<int, G_SIZE>& gaps) {
        ops_stats stats;

        for (int gap: gaps) {
            for (int i = gap; i < A_SIZE; i++)
            {
                stats.comparisons += 1;

                T temp = arr[i];
                int j = i;
                stats.assignments += 2;
                
                for (j; j >= gap && arr[j - gap] > temp; j -= gap)
                {
                    arr[j] = arr[j - gap];
                    stats.comparisons += 2;
                    stats.assignments += 2;
                }

                if (j < gap) {
                    stats.comparisons += 1;
                } else {
                    stats.comparisons += 2;
                }

                arr[j] = temp;
                stats.assignments += 1;
            }
        }

        return stats;
    }

    template <typename T, std::size_t A_SIZE, std::size_t G_SIZE>
    void shellsort_improved(std::array<T, A_SIZE>& arr, const std::array<int, G_SIZE>& gaps) {
        for (int gap: gaps) {
            for (int i = gap; i < A_SIZE; i++)
            {
                if (arr[i - gap] > arr[i]) {
                    T temp = arr[i];
                    int j = i;

                    do {
                        arr[j] = arr[j - gap];
                        j -= gap;

                    } while (j >= gap && arr[j - gap] > temp);
                    
                    arr[j] = temp;
                }
            }
        }
    }

    template <typename T, std::size_t A_SIZE, std::size_t G_SIZE>
    ops_stats shellsort_improved_reported(std::array<T, A_SIZE>& arr, const std::array<int, G_SIZE>& gaps) {
        ops_stats stats;

        for (int gap: gaps) {
            for (int i = gap; i < A_SIZE; i++)
            {
                stats.comparisons += 1;

                if (arr[i - gap] > arr[i]) {
                    T temp = arr[i];
                    int j = i;
                    stats.assignments += 2;

                    int loop_counter = 0;

                    do {
                        if (loop_counter > 0) {
                            stats.comparisons += 2;
                        }

                        arr[j] = arr[j - gap];
                        j -= gap;
                        stats.assignments += 2;

                        loop_counter++;
                    } while (j >= gap && arr[j - gap] > temp);

                    if (j < gap) {
                        stats.comparisons += 1;
                    } else {
                        stats.comparisons += 2;
                    }
                    
                    arr[j] = temp;
                    stats.assignments += 1;
                }
                stats.comparisons += 1;
            }
        }

        return stats;
    }
}