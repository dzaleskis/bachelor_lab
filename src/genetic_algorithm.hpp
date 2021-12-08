#pragma once

#include <fstream>
#include "json.hpp"
#include "solution.hpp"
#include "fitness.hpp"
#include "sorting_algorithm.hpp"
#include "openGA.hpp"

using json = nlohmann::json;

// TODO: FIX LATER
constexpr int RUNS = 3;
constexpr int SIZE = 1000;
const char* RESULTS_PATH = "results.json";

struct MiddleCost {
    SortingStats sortingStats;
    AvgSortingStats avgSortingStats;
};

struct OptimisationResult {
    MiddleCost cost;
    AlgorithmBlueprint algorithm;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(MiddleCost, sortingStats, avgSortingStats)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(OptimisationResult, algorithm, cost)

typedef EA::Genetic<Solution, MiddleCost> GAType;
typedef EA::GenerationType<Solution, MiddleCost> GenerationType;

void init_genes(Solution& s, const std::function<double(void)> &rnd01) {
    s.init_genes(rnd01);
}

Solution mutate(
        const Solution& X_base,
        const std::function<double(void)> &rnd01,
        double shrink_scale)
{
    return X_base.mutate(rnd01, shrink_scale);
}

Solution crossover(
        const Solution& X1,
        const Solution& X2,
        const std::function<double(void)> &rnd01)
{
    return X1.crossover(X2, rnd01);
}

bool eval_solution(const Solution& s, MiddleCost &c) {
    auto sortingStats = get_sorting_stats(s.algorithm, SIZE, RUNS);

    c.sortingStats = sortingStats;
    c.avgSortingStats = AvgSortingStats(sortingStats);

    return true; // genes are accepted
}

double calculate_SO_total_fitness(const GAType::thisChromosomeType &X)
{
    // finalize the cost
    return evaluate_fitness(X.middle_costs.avgSortingStats);
}

void SO_report_generation(
        int generation_number,
        const EA::GenerationType<Solution, MiddleCost> &last_generation,
        const Solution& best_genes)
{
    json j = {
            {"generation_number", generation_number},
            {"best_genes", best_genes.algorithm},
            {"best_total_cost", last_generation.best_total_cost},
            {"best_average_cost", last_generation.average_cost},
    };

    std::cout << j << std::endl;
}

void save_results(const GAType &ga_obj)
{
    std::ofstream output_file;
    output_file.open(RESULTS_PATH);
    int best_index = ga_obj.last_generation.best_chromosome_index;
    auto best_solution = ga_obj.last_generation.chromosomes[best_index];
    std::vector<OptimisationResult> results;
    results.push_back({best_solution.middle_costs, best_solution.genes.algorithm});

    json j = {
            {"results", results}
    };

    output_file << j << std::endl;
    output_file.close();
}
