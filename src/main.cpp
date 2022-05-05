#include <iostream>
#include <functional>
#include "json.hpp"
#include "openGA.hpp"
#include "genetic_algorithm.hpp"

using json = nlohmann::json;

struct GaConfig {
    int generations;
    int population;
    int size;
    double mut_rate;
    double crossover_frac;
};

void init_common_config(GAType& ga_obj, const GaConfig & config) {
    ga_obj.multi_threading=true;
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

void check_classic_stats(ClassicAlgorithm algorithm, int size, int runs) {
    SortStats stats = classic_sort_stats(algorithm, size, runs);

    json statsJson(stats);
    statsJson["algorithm"] = algorithm;
    std::cout << statsJson.dump(2) << std::endl;
}

void compare_algos(int size) {
    int runs = 1000;

    std::cout << "size: " << size << '\n';

    check_classic_stats(ClassicAlgorithm::INSERTION_SORT, size, runs);
    check_classic_stats(ClassicAlgorithm::SHELL_SORT, size, runs);
}

void compare_algos(int min_pow_2, int max_pow_2) {
    int max_size = 1 << max_pow_2;

    for (int size = 1 << min_pow_2; size <= max_size; size <<= 1) {
        compare_algos(size);
    }
}

int main(int argc, char* argv[]) {
	GaConfig config = {50, 500, 128, 0.1, 0.4 };

    try {
//        run_mo_ga(config);
//        compare_algos(4, 8);
        bench_cycles_classic(ClassicAlgorithm::INSERTION_SORT, 100, 1000);

    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

	return 0;
}