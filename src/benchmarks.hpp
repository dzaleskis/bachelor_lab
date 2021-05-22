#pragma once

#include <chrono>
#include "algorithms.hpp"
#include "iterator_utils.hpp"

namespace benchmarks {

    struct ops_bench {
        unsigned int classic_comparisons;
        unsigned int improved_comparisons;
        unsigned int classic_assignments;
        unsigned int improved_assignments;
    };

    struct time_bench {
        unsigned int classic_ms;
        unsigned int improved_ms;
    };


    template <std::size_t A_SIZE, std::size_t G_SIZE>
    time_bench measure_time_int(const std::vector<int>& gaps) {
        // create the original array
        std::vector<int> vec = std::vector<int>(A_SIZE);
        array_utils::fill_random(vec);

        // make copies, so both methods have same conditions
        auto vec1(vec);

        auto start1 = std::chrono::high_resolution_clock::now();
        algorithms::shellsort_classic(vec, gaps);
        auto stop1 = std::chrono::high_resolution_clock::now();
        auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1);

        auto start2 = std::chrono::high_resolution_clock::now();
        algorithms::shellsort_improved(vec1, gaps);
        auto stop2 = std::chrono::high_resolution_clock::now();
        auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(stop2 - start2);
        
        return {duration1.count(), duration2.count()};
    }
}