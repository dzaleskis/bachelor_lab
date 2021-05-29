#pragma once

#include <chrono>
#include "algorithms.hpp"
#include "iterator_utils.hpp"
#include <iostream>
#include <float.h>

namespace benchmarks {

    struct gap_seq_bench {
        std::vector<int> gap_seq;
        double avg_comparisons;
        double avg_assignments;
    };

    struct time_bench {
        unsigned int avg_millis;
        std::vector<int> best_time_seq;
    };

    template <typename T, class Compare>
    std::vector<T> sorted(std::vector<T> copy, Compare comp) {
        std::sort(copy.begin(), copy.end(), comp);
        return copy;
    }

    void print_gap_seq(const std::vector<int>& gap_seq) {
        for (const auto gap: gap_seq) {
            std::cout << gap << ", ";
        }

        std::cout << std::endl;
    }

    void print_gap_seq_bench_comparisons(const gap_seq_bench& gap_seq_bench) {
        std::cout << "1. avg. comparisons: " << gap_seq_bench.avg_comparisons << std::endl;
    }

    void print_gap_seq_bench_assignments(const gap_seq_bench& gap_seq_bench) {
        std::cout << "2. avg. assignments:" << gap_seq_bench.avg_assignments << std::endl;
    }

    void print_gap_seq_ops_bench(const gap_seq_bench& gap_seq_bench) {
        print_gap_seq(gap_seq_bench.gap_seq);
        print_gap_seq_bench_comparisons(gap_seq_bench);
        print_gap_seq_bench_assignments(gap_seq_bench);
        std::cout << std::endl;
    }

    void bench_classic_ops(const std::vector<std::vector<int>>& gap_seqs, int array_size, int runs) {
        std::vector<gap_seq_bench> gaps_stats;
        std::vector<std::vector<int>> vecs = std::vector<std::vector<int>>(runs, std::vector<int>(array_size));
        for (auto & vec: vecs) {
            iterator_utils::fill_random(vec.begin(), vec.end());
        }

        for (const auto & gap_seq: gap_seqs) {
            double total_comparisons = 0;
            double total_assignments = 0;

            for (int i = 0; i < runs; i++) {
                // create a copy to not modify original data
                std::vector<int> vec_copy = std::vector<int>(vecs[i]);
                // REPLACE WITH LAMBDA
                auto stats = algorithms::shellsort_classic_reported(vec_copy, gap_seq);
                total_comparisons += stats.comparisons;
                total_assignments += stats.assignments;
            }

            gaps_stats.push_back({ std::vector<int>(gap_seq), total_comparisons / runs, total_assignments / runs });
            print_gap_seq_ops_bench(gaps_stats.back());
        }

        std::sort(gaps_stats.begin(), gaps_stats.end(), [ ]( const gap_seq_bench& left, const gap_seq_bench& right )
        {
            return left.avg_comparisons < right.avg_comparisons;
        });

        std::cout << std::endl << "best comparisons" << std::endl;
        print_gap_seq(gaps_stats.front().gap_seq);
        print_gap_seq_bench_comparisons(gaps_stats.front());


        std::sort(gaps_stats.begin(), gaps_stats.end(), [ ]( const gap_seq_bench& left, const gap_seq_bench& right )
        {
            return left.avg_assignments < right.avg_assignments;
        });

        std::cout << std::endl << "best assignments" << std::endl;
        print_gap_seq(gaps_stats.front().gap_seq);
        print_gap_seq_bench_assignments(gaps_stats.front());
    }

//
//    template <std::size_t A_SIZE, std::size_t G_SIZE>
//    time_bench measure_time_int(const std::vector<int>& gaps) {
//        // create the original array
//        std::vector<int> vec = std::vector<int>(A_SIZE);
//        array_utils::fill_random(vec);
//
//        // make copies, so both methods have same conditions
//        auto vec1(vec);
//
//        auto start1 = std::chrono::high_resolution_clock::now();
//        algorithms::shellsort_classic(vec, gaps);
//        auto stop1 = std::chrono::high_resolution_clock::now();
//        auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1);
//
//        auto start2 = std::chrono::high_resolution_clock::now();
//        algorithms::shellsort_improved(vec1, gaps);
//        auto stop2 = std::chrono::high_resolution_clock::now();
//        auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(stop2 - start2);
//
//        return {duration1.count(), duration2.count()};
//    }
}