#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include "pass.hpp"
#include "gaps.hpp"
#include "json.hpp"
#include "fitness.hpp"
#include "sorting_algorithm.hpp"
#include "openGA.hpp"

using json = nlohmann::json;

const char* RESULT_PATH = "result.json";
const double IGNORED_OBJECTIVE = std::numeric_limits<double>::infinity();

// depends on data size (128 - 9000, 1024 - 125000, 8192 - 1500000)
const double CYCLES_THRESHOLD = 1500000;

typedef SortStats MiddleCost;
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

struct GeneticAlgorithmResult: SortStats {
    GeneticAlgorithmResult(const SortStats &stats, const Solution & solution, int size) : SortStats(stats) {
        this->solution = solution;
        this->size = size;
    }

    Solution solution;
    int size;
    // v1 - prioritize speed, v2 - prioritize compares, v3 - no priority
    int obj_version = 1;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GeneticAlgorithmResult, avg_inversions, avg_assignments, avg_comparisons, avg_cycles, solution, size, obj_version)

typedef EA::Genetic<Solution, MiddleCost> GAType;
typedef EA::GenerationType<Solution, MiddleCost> GenerationType;

class GeneticAlgorithm {
public:
    GeneticAlgorithm(int size) {
        this->size = size;
    }

    void init_genes(Solution& s, const std::function<double(void)> &rnd01) {
        std::vector<int> gaps = rand_chance(rnd01)
                ? get_geometric_gaps(size, rand_num(rnd01, 1.5, 7))
                : get_random_gaps(size, [&]() { return rand_num(rnd01, 1.5, 12.5); });
        s.passBlueprints.reserve(gaps.size());

        for (int & gap: gaps) {
            auto pass = rand_pass(rnd01);
            s.passBlueprints.emplace_back(pass, gap);
        }
    }

    bool eval_solution(const Solution& s, MiddleCost &c) {
        c = blueprint_sort_stats(s, size, 3);

        if (c.avg_inversions > size / 10) {
            return false;
        }

        if (c.avg_cycles > CYCLES_THRESHOLD * 2) {
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
        int min_size = std::min(s1.passBlueprints.size(), s2.passBlueprints.size());

        for (int i = 0; i < min_size; ++i) {
            if (rand_chance(rnd01)) {
                s_new.passBlueprints.emplace_back(s1.passBlueprints.at(i));
            } else {
                s_new.passBlueprints.emplace_back(s2.passBlueprints.at(i));
            }
        }

        // sort just in case
        std::sort(s_new.passBlueprints.begin(), s_new.passBlueprints.end());

        return std::move(s_new);
    }

    virtual void save_results(const GAType &ga_obj) = 0;

protected:
    int size;
};

class MOGeneticAlgorithm: public GeneticAlgorithm {
public:
    MOGeneticAlgorithm(int size) : GeneticAlgorithm(size) {}

    std::vector<double> calculate_objectives(const GAType::thisChromosomeType &x)
    {
        auto inversions = x.middle_costs.avg_inversions;
        auto cycles = x.middle_costs.avg_cycles;
        auto comparisons = x.middle_costs.avg_comparisons;
        auto assignments = x.middle_costs.avg_assignments;

        if (inversions > 0) {
            return {
                    inversions,
                    IGNORED_OBJECTIVE,
                    IGNORED_OBJECTIVE,
                    IGNORED_OBJECTIVE
            };
        }


        if (cycles > CYCLES_THRESHOLD) {
            return {
                    inversions,
                    cycles,
                    IGNORED_OBJECTIVE,
                    IGNORED_OBJECTIVE,
            };
        }

        return {
                inversions,
                cycles,
                comparisons,
                assignments,
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
