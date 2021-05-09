#pragma once

#include <chrono>
#include "json.hpp"
#include "algorithms.hpp"
#include "array_utils.hpp"

namespace benchmarks {

    using json = nlohmann::json;

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
    time_bench measure_time_int(const std::array<int, G_SIZE>& gaps) {
        // create the original array
        std::array<int, A_SIZE> mother_arr;
        array_utils::fill_random(mother_arr);

        // make copies, so both methods have same conditions
        auto arr1 = mother_arr;
        auto arr2 = mother_arr;

        auto start1 = std::chrono::high_resolution_clock::now();
        algorithms::shellsort_classic(arr1, gaps);
        auto stop1 = std::chrono::high_resolution_clock::now();
        auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1);

        auto start2 = std::chrono::high_resolution_clock::now();
        algorithms::shellsort_improved(arr2, gaps);
        auto stop2 = std::chrono::high_resolution_clock::now();
        auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(stop2 - start2);
        
        return {duration1.count(), duration2.count()};
    }

    template <std::size_t A_SIZE, std::size_t G_SIZE>
    ops_bench measure_ops_int(const std::array<int, G_SIZE>& gaps) {
        // create the original array
        std::array<int, A_SIZE> mother_arr;
        array_utils::fill_random(mother_arr);

        // make copies, so both methods have same conditions
        auto arr1 = mother_arr;
        auto arr2 = mother_arr;

        auto stats1 = algorithms::shellsort_classic_reported(arr1, gaps);
        auto stats2 = algorithms::shellsort_improved_reported(arr2, gaps);

        return {stats1.comparisons, stats2.comparisons, stats1.assignments, stats2.assignments};
    }

}