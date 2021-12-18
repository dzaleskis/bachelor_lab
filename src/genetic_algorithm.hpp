#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include "pass.hpp"
#include "random_utils.hpp"
#include "gaps.hpp"
#include "json.hpp"
#include "fitness.hpp"
#include "sorting_algorithm.hpp"
#include "openGA.hpp"

using json = nlohmann::json;

constexpr int EVAL_RUNS = 1;
constexpr int CONTAINER_SIZE = 10000;
constexpr double EQUAL_PROBABILITY = 0.5;

const char* RESULT_PATH = "result.json";

typedef SortingStats MiddleCost;
typedef AlgorithmBlueprint Solution;

inline bool withProbability(double probability, const std::function<double(void)> &rnd01) {
    return probability < rnd01();
}

inline std::vector<int> get_random_gaps(const std::function<double(void)> &rnd01) {
    int random_gap_index = std::floor(rnd01() * ALL_GAPS.size());
    auto gaps = ALL_GAPS.at(random_gap_index);

    return std::move(gaps);
}

inline PassType get_random_pass(const std::function<double(void)> &rnd01) {
    int random_pass_index = std::floor(rnd01() * ALL_PASSES.size());
    auto pass = ALL_PASSES.at(random_pass_index);

    return pass;
}

inline void normalize(Solution& s) {
    std::sort(s.passBlueprints.begin(), s.passBlueprints.end(), std::greater<>());
}

struct GeneticAlgorithmResult: SortingStats {
    GeneticAlgorithmResult(const SortingStats &stats, double fitness, const Solution & s) : SortingStats(stats) {
        this->fitness = fitness;
        this->solution = s;
    }

    double fitness;
    Solution solution;
    int size = CONTAINER_SIZE;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GeneticAlgorithmResult, avg_inversions, avg_assignments, avg_comparisons, avg_time, fitness, solution, size)

typedef EA::Genetic<Solution, MiddleCost> GAType;
typedef EA::GenerationType<Solution, MiddleCost> GenerationType;

void init_genes(Solution& s, const std::function<double(void)> &rnd01) {
    auto gaps = get_random_gaps(rnd01);
    s.passBlueprints.reserve(gaps.size());

    for (int & gap : gaps) {
        auto pass = get_random_pass(rnd01);
        s.passBlueprints.emplace_back(pass, gap);
    }

    normalize(s);
}

Solution mutate(
        const Solution& s_base,
        const std::function<double(void)> &rnd01,
        double shrink_scale)
{
    Solution s_new(s_base);

    int random_blueprint_index = std::floor(rnd01() * s_new.passBlueprints.size());
    auto & blueprint = s_new.passBlueprints.at(random_blueprint_index);

    // mutate pass type (pick a random new one)
    blueprint.passType = get_random_pass(rnd01);

    return std::move(s_new);
}

Solution crossover(
        const Solution& s1,
        const Solution& s2,
        const std::function<double(void)> &rnd01)
{
    Solution s_new(s1);

    // using uniform distribution
    // assumes s1 size == s2 size
    for (int i = 0; i < s2.passBlueprints.size(); i++) {
        if (withProbability(EQUAL_PROBABILITY, rnd01)) {
            s_new.passBlueprints.at(i) = s2.passBlueprints.at(i);
        }
    }

    normalize(s_new);

    return std::move(s_new);
}

bool eval_solution(const Solution& s, MiddleCost &c) {
    c = get_blueprint_sorting_stats(s, CONTAINER_SIZE, EVAL_RUNS);

    return true; // genes are accepted
}

double calculate_SO_total_fitness(const GAType::thisChromosomeType &X)
{
    // finalize the cost
    return evaluate_fitness(X.middle_costs);
}

void SO_report_generation(
        int generation_number,
        const EA::GenerationType<Solution, MiddleCost> &last_generation,
        const Solution& best_genes)
{
    json j = {
            {"generation_number", generation_number},
            {"best_total_cost", last_generation.best_total_cost},
            {"average_cost", last_generation.average_cost},
    };

    std::cout << j.dump(2) << std::endl;
}

void save_results(const GAType &ga_obj)
{
    std::ofstream output_file;
    output_file.open(RESULT_PATH);

    int best_index = ga_obj.last_generation.best_chromosome_index;
    auto best_solution = ga_obj.last_generation.chromosomes[best_index];

    GeneticAlgorithmResult result(best_solution.middle_costs, best_solution.total_cost, best_solution.genes);

    json resultJson(result);

    // print to console (pretty)
    std::cout << resultJson.dump(2) << std::endl;

    // save to file
    output_file << resultJson << std::endl;
    output_file.close();
}
