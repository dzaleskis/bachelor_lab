#pragma once
#include <array>
#include "elements.hpp"

namespace algorithms {

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
    stats shellsort_improved(std::array<T, A_SIZE>& arr, const std::array<int, G_SIZE>& gaps) {
        std::string measure_key = global_measure.init_report();

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

        return global_measure.get_report(measure_key);
    }
}