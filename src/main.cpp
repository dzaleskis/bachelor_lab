#include <iostream>
#include <functional>
#include "json.hpp"
#include "openGA.hpp"
#include "genetic_algorithm.hpp"
#include "classic_algorithms.hpp"
#include "benchmarks.hpp"

using json = nlohmann::json;

struct GaConfig {
    int generations;
    int population;
    int size;
    double mut_rate;
    double crossover_frac;
};

void init_common_config(GAType& ga_obj, const GaConfig & config) {
    ga_obj.multi_threading=false;
    ga_obj.idle_delay_us=1;
    ga_obj.verbose=false;
    ga_obj.population=config.population;
    ga_obj.generation_max=config.generations;
    ga_obj.crossover_fraction=config.crossover_frac;
    ga_obj.mutation_rate=config.mut_rate;
}

void init_mo_config(GAType& ga_obj, MOGeneticAlgorithm& algorithm) {
    ga_obj.problem_mode=EA::GA_MODE::NSGA_III;

    ga_obj.init_genes=[&](Solution& s, const std::function<double(void)> &rnd01) { return algorithm.init_genes(s, rnd01);};
    ga_obj.eval_solution=[&](const Solution& s, MiddleCost &c) { return algorithm.eval_solution(s, c);};
    ga_obj.mutate=[&](const Solution& s_base, const std::function<double(void)> &rnd01, double shrink_scale) { return algorithm.mutate(s_base, rnd01, shrink_scale);};
    ga_obj.crossover=[&](const Solution& s1, const Solution& s2, const std::function<double(void)> &rnd01) { return algorithm.crossover(s1, s2, rnd01);};
    ga_obj.calculate_MO_objectives=[&](const GAType::thisChromosomeType &x) { return algorithm.calculate_objectives(x);};
    ga_obj.MO_report_generation=[&](int gen, const EA::GenerationType<Solution,MiddleCost> &last_gen, const std::vector<unsigned int>& p_front) { return algorithm.report_generation(gen, last_gen, p_front);};
}


void run_mo_ga(const GaConfig & config) {
    std::cout << "Starting genetic algorithm" << std::endl;

    GAType ga_obj;
    MOGeneticAlgorithm algorithm(config.size);

    init_common_config(ga_obj, config);
    init_mo_config(ga_obj, algorithm);

    ga_obj.solve();

    algorithm.save_results(ga_obj);

    std::cout << "Genetic algorithm ran to completion" << std::endl;
}

void check_classic_algo() {
    auto sort_fn = [&](std::vector<int>& data) {
        experimental_shell_sort(data);
    };

    auto traced_sort_fn = [&](std::vector<Element<int>>& data) {
        experimental_shell_sort(data);
    };

    auto stats = sort_stats(sort_fn, traced_sort_fn, 64, 5000000);

    json statsJson(stats);
    std::cout << statsJson.dump(2) << std::endl;
}

int main(int argc, char* argv[]) {
	GaConfig config = {50, 500, 128, 0.1, 0.4 };

    try {
//        run_mo_ga(config);
//        check_classic_algo();
        bench_int();
//        bench_for_perf();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

	return 0;
}