#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "json.hpp"
#include "sorting_algorithm.hpp"
#include "gaps.hpp"
#include "fitness.hpp"
#include "iterator_utils.hpp"
#include "benchmark_algorithms.hpp"

struct BenchmarkResult: SortingStats {
    BenchmarkResult(const SortingStats &stats, int size, std::string algorithm_name) : SortingStats(stats) {
        this->size = size;
        this->algorithm_name = algorithm_name;
    }

    int size;
    std::string algorithm_name;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(BenchmarkResult, avg_assignments, avg_comparisons, avg_time, algorithm_name, size)

SortingStats run_algorithm_benchmark(AlgorithmBlueprint algorithmBlueprint, std::vector<std::vector<int>> allData) {
    auto rawAlgorithm = ConcreteAlgorithmFactory::getConcreteAlgorithm<std::vector<int>>(algorithmBlueprint);
    auto elementAlgorithm = ConcreteAlgorithmFactory::getConcreteAlgorithm<std::vector<Element<int>>>(algorithmBlueprint);

    double total_comparisons = 0;
    double total_assignments = 0;
    double total_time = 0;

    for (auto &data: allData) {
        std::vector<int> raw(data.begin(), data.end());
        std::vector<Element<int>> elements(data.begin(), data.end());

        auto time = measure_time_us([&]() {
            rawAlgorithm->sort(raw);
        });

        auto report = global_measure.withReport([&]() {
            elementAlgorithm->sort(elements);
        });

        total_time += time;
        total_comparisons += report.comparisons;
        total_assignments += report.assignments;
    }

    auto runs = allData.size();
    auto stats = SortingStats { 0, total_comparisons / runs, total_assignments / runs, total_time / runs };

    return stats;
}

void run_benchmark(int runs, const std::vector<int>& sizes, const char* outputPath) {
    std::vector<BenchmarkResult> results;

    std::ofstream output_file;
    output_file.open(outputPath);

    for (auto size: sizes) {
        std::vector<std::vector<int>> allData = std::vector<std::vector<int>>(runs, std::vector<int>(size));
        for (auto & data: allData) {
            utils::fill_random(data.begin(), data.end());
        }

        for (auto & benchmarkAlgorithm: ALL_SMALL_ALGORITHMS) {
            auto stats = run_algorithm_benchmark(benchmarkAlgorithm.algorithmBlueprint, allData);
            BenchmarkResult result(stats, size, benchmarkAlgorithm.name);

            results.push_back(result);

            json resultJson(result);
            std::cout << resultJson.dump(2) << std::endl;
        }
    }

    json resultsJson(results);
    output_file << resultsJson << std::endl;
    output_file.close();
}

void run_all_benchmarks() {
    // RUN SMALL
    run_benchmark(10000, {125, 250, 500, 1000}, "benchmark_results_small.json");

    // RUN MID
    run_benchmark(1000, {1250, 2500, 5000, 10000}, "benchmark_results_mid.json");
}