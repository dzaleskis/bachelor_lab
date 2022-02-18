#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include "pass.hpp"
#include "random_utils.hpp"
#include "gaps.hpp"
#include "json.hpp"
#include "stats.hpp"
#include "sorting_algorithm.hpp"
#include "openGA.hpp"

using json = nlohmann::json;

const double EQUAL_PROBABILITY = 0.5;
const char* RESULT_PATH = "result.json";

typedef SortingStats MiddleCost;
typedef AlgorithmBlueprint Solution;

inline bool withProbability(double probability, const std::function<double(void)> &rnd01) {
    return probability < rnd01();
}

inline PassType get_random_pass(const std::function<double(void)> &rnd01) {
    int random_pass_index = std::floor(rnd01() * ALL_PASSES.size());
    return ALL_PASSES.at(random_pass_index);
}

struct GeneticAlgorithmResult: SortingStats {
    GeneticAlgorithmResult(const SortingStats &stats, double fitness, const Solution & solution, int size) : SortingStats(stats) {
        this->fitness = fitness;
        this->solution = solution;
        this->size = size;
    }

    double fitness;
    Solution solution;
    int size;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GeneticAlgorithmResult, avg_inversions, avg_assignments, avg_comparisons, avg_cycles, fitness, solution, size)

typedef EA::Genetic<Solution, MiddleCost> GAType;
typedef EA::GenerationType<Solution, MiddleCost> GenerationType;

class GeneticAlgorithm {
public:
    GeneticAlgorithm(int size, int runs) {
        this->size = size;
        this->runs = runs;
    }

    void init_genes(Solution& s, const std::function<double(void)> &rnd01) {
        auto gaps = get_geometric_gaps(size, 2.25);
        s.passBlueprints.reserve(gaps.size());

        for (int & gap: gaps) {
            auto pass = get_random_pass(rnd01);
            s.passBlueprints.emplace_back(pass, gap);
        }
    }

    bool eval_solution(const Solution& s, MiddleCost &c) {
        c = get_sorting_stats(s, size, runs);

        return true; // genes are accepted
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
        int min_size = std::min(s1.passBlueprints.size(), s2.passBlueprints.size());

        for (int i = 0; i < min_size; i++) {
            if (withProbability(EQUAL_PROBABILITY, rnd01)) {
                s_new.passBlueprints.at(i).passType = s2.passBlueprints.at(i).passType;
            }
        }

        return std::move(s_new);
    }

    virtual void save_results(const GAType &ga_obj) = 0;

protected:
    int size;
    int runs;
};

class MOGeneticAlgorithm: public GeneticAlgorithm {
public:
    MOGeneticAlgorithm(int size, int runs) : GeneticAlgorithm(size, runs) {}

    std::vector<double> calculate_objectives(const GAType::thisChromosomeType &x)
    {
        return {
                std::pow(x.middle_costs.avg_inversions, 2),
                x.middle_costs.avg_comparisons,
                x.middle_costs.avg_assignments,
                x.middle_costs.avg_cycles,
        };
    }

    void report_generation(
            int generation,
            const EA::GenerationType<Solution,MiddleCost> &last_generation,
            const std::vector<unsigned int>& pareto_front)
    {
        const auto& first = last_generation.chromosomes[pareto_front[0]];

        json j = {
                {"generation", generation},
                {"pareto_front_size", pareto_front.size()},
                {"first_costs", first.middle_costs},
                {"first_objectives", first.objectives},
        };

        std::cout << j.dump(2) << std::endl;
    }

    void save_results(const GAType &ga_obj)
    {
        std::ofstream output_file;
        output_file.open(RESULT_PATH);

        std::vector<unsigned int> frontIndices = ga_obj.last_generation.fronts[0];
        std::vector<GeneticAlgorithmResult> results;

        for (auto i: frontIndices) {
            const auto &solution = ga_obj.last_generation.chromosomes[i];
            GeneticAlgorithmResult result(solution.middle_costs, solution.total_cost, solution.genes, size);

            results.emplace_back(result);
        }

        json resultJson(results);

        // print to console (pretty)
        std::cout << resultJson.dump(2) << std::endl;

        // save to file
        output_file << resultJson << std::endl;
        output_file.close();
    }

};
