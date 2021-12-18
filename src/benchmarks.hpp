#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "json.hpp"
#include "sorting_algorithm.hpp"
#include "gaps.hpp"
#include "fitness.hpp"

struct BenchmarkResult: SortingStats {
    BenchmarkResult(const SortingStats &stats, int size, std::string algorithm_name) : SortingStats(stats) {
        this->size = size;
        this->algorithm_name = algorithm_name;
    }

    int size;
    std::string algorithm_name;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(BenchmarkResult, avg_assignments, avg_comparisons, avg_time, algorithm_name, size)


struct BenchmarkAlgorithm {
    BenchmarkAlgorithm(AlgorithmBlueprint algorithmBlueprint, const std::string & name) {
        this->algorithmBlueprint = algorithmBlueprint;
        this->name = name;
    }

    AlgorithmBlueprint algorithmBlueprint;
    std::string name;
};

AlgorithmBlueprint construct_algorithm(PassType pass, const std::vector<int> & gaps) {
    AlgorithmBlueprint algorithmBlueprint;
    algorithmBlueprint.passBlueprints.reserve(gaps.size());

    for (int gap : gaps) {
        algorithmBlueprint.passBlueprints.emplace_back(pass, gap);
    }

    return std::move(algorithmBlueprint);
}

// small algorithms
BenchmarkAlgorithm SH_SMALL_CIURA = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION, CIURA_SMALL_GAPS), "SH_SMALL_CIURA");
BenchmarkAlgorithm SH_SMALL_TOKUDA = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION, TOKUDA_SMALL_GAPS), "SH_SMALL_TOKUDA");
BenchmarkAlgorithm SH_SMALL_SEDGEWICK = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION, SEDGEWICK_SMALL_GAPS), "SH_SMALL_SEDGEWICK");
BenchmarkAlgorithm SH_SMALL_ICSW = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION, INCERPI_SEDGEWICK_SMALL_GAPS), "SH_SMALL_ICSW");
BenchmarkAlgorithm SH_SMALL_PRATT = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION, PRATT_SMALL_GAPS), "SH_SMALL_PRATT");

BenchmarkAlgorithm SHIMP_SMALL_CIURA = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, CIURA_SMALL_GAPS), "SHIMP_SMALL_CIURA");
BenchmarkAlgorithm SHIMP_SMALL_TOKUDA = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, TOKUDA_SMALL_GAPS), "SHIMP_SMALL_TOKUDA");
BenchmarkAlgorithm SHIMP_SMALL_SEDGEWICK = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, SEDGEWICK_SMALL_GAPS), "SHIMP_SMALL_SEDGEWICK");
BenchmarkAlgorithm SHIMP_SMALL_ICSW = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, INCERPI_SEDGEWICK_SMALL_GAPS), "SHIMP_SMALL_ICSW");
BenchmarkAlgorithm SHIMP_SMALL_PRATT = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, PRATT_SMALL_GAPS), "SHIMP_SMALL_PRATT");

const std::vector<BenchmarkAlgorithm> ALL_SMALL_ALGORITHMS {
    SH_SMALL_CIURA,SH_SMALL_TOKUDA,SH_SMALL_SEDGEWICK,SH_SMALL_ICSW,SH_SMALL_PRATT,
    SHIMP_SMALL_CIURA,SHIMP_SMALL_TOKUDA,SHIMP_SMALL_SEDGEWICK,SHIMP_SMALL_ICSW,SHIMP_SMALL_PRATT,
    // TODO: add genetics here
};

// mid algorithms
BenchmarkAlgorithm SH_MID_CIURA = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION, CIURA_MID_GAPS), "");
BenchmarkAlgorithm SH_MID_TOKUDA = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION, TOKUDA_MID_GAPS), "");
BenchmarkAlgorithm SH_MID_SEDGEWICK = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION, SEDGEWICK_MID_GAPS), "");
BenchmarkAlgorithm SH_MID_ICSW = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION, INCERPI_SEDGEWICK_MID_GAPS), "");
BenchmarkAlgorithm SH_MID_PRATT = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION, PRATT_MID_GAPS), "");

BenchmarkAlgorithm SHIMP_MID_CIURA = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, CIURA_MID_GAPS), "");
BenchmarkAlgorithm SHIMP_MID_TOKUDA = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, TOKUDA_MID_GAPS), "");
BenchmarkAlgorithm SHIMP_MID_SEDGEWICK = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, SEDGEWICK_MID_GAPS), "");
BenchmarkAlgorithm SHIMP_MID_ICSW = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, INCERPI_SEDGEWICK_MID_GAPS), "");
BenchmarkAlgorithm SHIMP_MID_PRATT = BenchmarkAlgorithm(construct_algorithm(PassType::INSERTION_IMPROVED, PRATT_MID_GAPS), "");

const std::vector<BenchmarkAlgorithm> ALL_MID_ALGORITHMS {
        SH_MID_CIURA,SH_MID_TOKUDA,SH_MID_SEDGEWICK,SH_MID_ICSW,SH_MID_PRATT,
        SHIMP_MID_CIURA,SHIMP_MID_TOKUDA,SHIMP_MID_SEDGEWICK,SHIMP_MID_ICSW,SHIMP_MID_PRATT,
        // TODO: add genetics here
};

void run_small_benchmarks(int runs) {
    // TODO: more sizes
    int size = 1000;

    for (auto & benchmarkAlgorithm: ALL_SMALL_ALGORITHMS) {
        auto stats = get_blueprint_sorting_stats(benchmarkAlgorithm.algorithmBlueprint, size, runs);
        BenchmarkResult result(stats, size, benchmarkAlgorithm.name);

        json j(result);
        std::cout << j.dump(2) << std::endl;
        // TODO: save to file, then dump to DB
    }
}

void run_mid_benchmarks(int runs) {
    // TODO: adapt from small benchmark and reuse code
}