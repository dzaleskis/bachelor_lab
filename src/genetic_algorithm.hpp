#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include "pass.hpp"
#include "random_utils.hpp"
#include "gaps.hpp"
#include "json.hpp"
#include "stats.hpp"
#include "sorting_algorithm.hpp"
#include "openGA.hpp"

using json = nlohmann::json;

const char* RESULT_PATH = "result.json";

// TODO: these should adjust to size of data
const double INVERSIONS_THRESHOLD = 5;
const double ACCEPTABLE_INVERSIONS = 50;

const double CYCLES_THRESHOLD = 4000;
const double ACCEPTABLE_CYCLES = 8000;

typedef SortingStats MiddleCost;
typedef AlgorithmBlueprint Solution;

inline bool rand_chance(const std::function<double(void)> &rnd01) {
    return 0.5 < rnd01();
}

inline double rand_num(const std::function<double(void)> &rnd01, double min, double max) {
    return rnd01() * (max - min) + min;
}

template<typename T>
inline int rand_index(const std::function<double(void)> &rnd01, const T& value) {
    return std::floor(rand_num(rnd01, 0, value.size()));
}

inline PassType rand_pass(const std::function<double(void)> &rnd01) {
    return ALL_PASSES.at(rand_index(rnd01, ALL_PASSES));
}

struct GeneticAlgorithmResult: SortingStats {
    GeneticAlgorithmResult(const SortingStats &stats, const Solution & solution, int size) : SortingStats(stats) {
        this->solution = solution;
        this->size = size;
    }

    Solution solution;
    int size;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GeneticAlgorithmResult, avg_inversions, avg_assignments, avg_comparisons, avg_cycles, solution, size)

typedef EA::Genetic<Solution, MiddleCost> GAType;
typedef EA::GenerationType<Solution, MiddleCost> GenerationType;

class GeneticAlgorithm {
public:
    GeneticAlgorithm(int size, int runs) {
        this->size = size;
        this->runs = runs;
    }

    void init_genes(Solution& s, const std::function<double(void)> &rnd01) {
        auto gaps = get_geometric_gaps(size, rand_num(rnd01, 1.2, 3.3));
        s.passBlueprints.reserve(gaps.size());

        for (int & gap: gaps) {
            auto pass = rand_pass(rnd01);
            s.passBlueprints.emplace_back(pass, gap);
        }
    }

    bool eval_solution(const Solution& s, MiddleCost &c) {
        c = get_sorting_stats(s, size, runs);

        if (c.avg_inversions > ACCEPTABLE_INVERSIONS) {
            return false;
        }

        if (c.avg_cycles > ACCEPTABLE_CYCLES) {
            return false;
        }

        return true; // genes are accepted
    }

    Solution mutate(
            const Solution& s_base,
            const std::function<double(void)> &rnd01,
            double shrink_scale)
    {
        Solution s_new(s_base);

        // mutate pass type (pick a random new one)
        int random_blueprint_index = rand_index(rnd01, s_new.passBlueprints);
        s_new.passBlueprints.at(random_blueprint_index).passType = rand_pass(rnd01);

        return std::move(s_new);
    }

    Solution crossover(
            const Solution& s1,
            const Solution& s2,
            const std::function<double(void)> &rnd01)
    {
        Solution s_new;
        int max_size = std::min(s1.passBlueprints.size(), s2.passBlueprints.size());

        for (int i = 0; i < max_size; ++i) {
            if (rand_chance(rnd01)) {
                s_new.passBlueprints.emplace_back(s1.passBlueprints.at(i));
            } else {
                s_new.passBlueprints.emplace_back(s2.passBlueprints.at(i));
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
        // first, prioritize inversions
        if (x.middle_costs.avg_inversions > INVERSIONS_THRESHOLD) {
            return {
                    x.middle_costs.avg_inversions,
                    std::numeric_limits<double>::infinity(),
                    std::numeric_limits<double>::infinity(),
                    std::numeric_limits<double>::infinity(),
            };
        }

        // then prioritize cycle count
        if (x.middle_costs.avg_cycles > CYCLES_THRESHOLD) {
            return {
                    x.middle_costs.avg_inversions,
                    std::numeric_limits<double>::infinity(),
                    std::numeric_limits<double>::infinity(),
                    x.middle_costs.avg_cycles,
            };
        }

        return {
                x.middle_costs.avg_inversions,
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
        json j = {
                {"generation", generation},
                {"pareto_front_size", pareto_front.size()},
        };

        std::cout << j.dump(2) << std::endl;
    }

    void save_results(const GAType &ga_obj)
    {
        std::ofstream output_file;
        output_file.open(RESULT_PATH);

        const auto& frontIndices = ga_obj.last_generation.fronts[0];
        std::vector<GeneticAlgorithmResult> results;

        for (auto i: frontIndices) {
            const auto &solution = ga_obj.last_generation.chromosomes[i];
            GeneticAlgorithmResult result(solution.middle_costs, solution.genes, size);

            results.emplace_back(result);
        }

        json resultJson(results);
        auto prettyJson = resultJson.dump(2);

        // print to console
        std::cout << prettyJson << std::endl;

        // save to file
        output_file << prettyJson << std::endl;
        output_file.close();
    }
};
