#pragma once

#include <chrono>
#include "algorithms.hpp"
#include "iterator_utils.hpp"
#include <iostream>
#include <float.h>

namespace benchmarks {

    struct gap_seq_ops_bench {
        std::vector<int> gap_seq;
        double avg_comparisons;
        double avg_assignments;
    };

    struct gap_seq_time_bench {
        std::vector<int> gap_seq;
        double avg_micros;
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

    void print_gap_seq_bench_comparisons(const gap_seq_ops_bench& gap_seq_bench) {
        std::cout << "1. avg. comparisons: " << gap_seq_bench.avg_comparisons << std::endl;
    }

    void print_gap_seq_bench_assignments(const gap_seq_ops_bench& gap_seq_bench) {
        std::cout << "2. avg. assignments:" << gap_seq_bench.avg_assignments << std::endl;
    }

    void print_gap_seq_bench_time(const gap_seq_time_bench& gap_seq_bench) {
        std::cout << "3. avg. time (us):" << gap_seq_bench.avg_micros << std::endl;
    }

    void print_gap_seq_ops_bench(const gap_seq_ops_bench& gap_seq_bench) {
        print_gap_seq(gap_seq_bench.gap_seq);
        print_gap_seq_bench_comparisons(gap_seq_bench);
        print_gap_seq_bench_assignments(gap_seq_bench);
        std::cout << std::endl;
    }

    void print_gap_seq_time_bench(const gap_seq_time_bench& gap_seq_bench) {
        print_gap_seq(gap_seq_bench.gap_seq);
        print_gap_seq_bench_time(gap_seq_bench);
        std::cout << std::endl;
    }

    void bench_classic_ops(const std::vector<std::vector<int>>& gap_seqs, int array_size, int runs) {
        std::vector<gap_seq_ops_bench> gaps_stats;
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

        std::sort(gaps_stats.begin(), gaps_stats.end(), [ ](const gap_seq_ops_bench& left, const gap_seq_ops_bench& right )
        {
            return left.avg_comparisons < right.avg_comparisons;
        });

        std::cout << std::endl << "best comparisons" << std::endl;
        print_gap_seq(gaps_stats.front().gap_seq);
        print_gap_seq_bench_comparisons(gaps_stats.front());


        std::sort(gaps_stats.begin(), gaps_stats.end(), [ ](const gap_seq_ops_bench& left, const gap_seq_ops_bench& right )
        {
            return left.avg_assignments < right.avg_assignments;
        });

        std::cout << std::endl << "best assignments" << std::endl;
        print_gap_seq(gaps_stats.front().gap_seq);
        print_gap_seq_bench_assignments(gaps_stats.front());
    }


    void bench_classic_time(const std::vector<std::vector<int>>& gap_seqs, int array_size, int runs) {
        std::vector<gap_seq_time_bench> gaps_stats;
        std::vector<std::vector<int>> vecs = std::vector<std::vector<int>>(runs, std::vector<int>(array_size));
        for (auto & vec: vecs) {
            iterator_utils::fill_random(vec.begin(), vec.end());
        }

        for (const auto & gap_seq: gap_seqs) {
            double total_time = 0;

            for (int i = 0; i < runs; i++) {
                // create a copy to not modify original data
                std::vector<int> vec_copy = std::vector<int>(vecs[i]);
                auto start = std::chrono::high_resolution_clock::now();
                algorithms::shellsort_classic(vec_copy, gap_seq);
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                total_time += duration.count();
            }

            gaps_stats.push_back({ std::vector<int>(gap_seq), total_time / runs });
            print_gap_seq_time_bench(gaps_stats.back());
        }

        std::sort(gaps_stats.begin(), gaps_stats.end(), [ ](const gap_seq_time_bench& left, const gap_seq_time_bench& right )
        {
            return left.avg_micros < right.avg_micros;
        });

        std::cout << std::endl << "best time" << std::endl;
        print_gap_seq(gaps_stats.front().gap_seq);
        print_gap_seq_bench_time(gaps_stats.front());
    }

    void bench_improved_ops(const std::vector<std::vector<int>>& gap_seqs, int array_size, int runs) {
        std::vector<gap_seq_ops_bench> gaps_stats;
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
                auto stats = algorithms::shellsort_improved_reported(vec_copy, gap_seq);
                total_comparisons += stats.comparisons;
                total_assignments += stats.assignments;
            }

            gaps_stats.push_back({ std::vector<int>(gap_seq), total_comparisons / runs, total_assignments / runs });
            print_gap_seq_ops_bench(gaps_stats.back());
        }

        std::sort(gaps_stats.begin(), gaps_stats.end(), [ ](const gap_seq_ops_bench& left, const gap_seq_ops_bench& right )
        {
            return left.avg_comparisons < right.avg_comparisons;
        });

        std::cout << std::endl << "best comparisons" << std::endl;
        print_gap_seq(gaps_stats.front().gap_seq);
        print_gap_seq_bench_comparisons(gaps_stats.front());


        std::sort(gaps_stats.begin(), gaps_stats.end(), [ ](const gap_seq_ops_bench& left, const gap_seq_ops_bench& right )
        {
            return left.avg_assignments < right.avg_assignments;
        });

        std::cout << std::endl << "best assignments" << std::endl;
        print_gap_seq(gaps_stats.front().gap_seq);
        print_gap_seq_bench_assignments(gaps_stats.front());
    }


    void bench_improved_time(const std::vector<std::vector<int>>& gap_seqs, int array_size, int runs) {
        std::vector<gap_seq_time_bench> gaps_stats;
        std::vector<std::vector<int>> vecs = std::vector<std::vector<int>>(runs, std::vector<int>(array_size));
        for (auto & vec: vecs) {
            iterator_utils::fill_random(vec.begin(), vec.end());
        }

        for (const auto & gap_seq: gap_seqs) {
            double total_time = 0;

            for (int i = 0; i < runs; i++) {
                // create a copy to not modify original data
                std::vector<int> vec_copy = std::vector<int>(vecs[i]);
                auto start = std::chrono::high_resolution_clock::now();
                algorithms::shellsort_improved(vec_copy, gap_seq);
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                total_time += duration.count();
            }

            gaps_stats.push_back({ std::vector<int>(gap_seq), total_time / runs });
            print_gap_seq_time_bench(gaps_stats.back());
        }

        std::sort(gaps_stats.begin(), gaps_stats.end(), [ ](const gap_seq_time_bench& left, const gap_seq_time_bench& right )
        {
            return left.avg_micros < right.avg_micros;
        });

        std::cout << std::endl << "best time" << std::endl;
        print_gap_seq(gaps_stats.front().gap_seq);
        print_gap_seq_bench_time(gaps_stats.front());
    }
}