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
    using std::placeholders::_1;
    using std::placeholders::_2;
    using std::placeholders::_3;

    ga_obj.problem_mode=EA::GA_MODE::NSGA_III;

    ga_obj.init_genes = std::bind(&GeneticAlgorithm::init_genes, &algorithm, _1, _2);
    ga_obj.eval_solution = std::bind(&GeneticAlgorithm::eval_solution, &algorithm, _1, _2);
    ga_obj.mutate = std::bind(&GeneticAlgorithm::mutate, &algorithm, _1, _2, _3);
    ga_obj.crossover = std::bind(&GeneticAlgorithm::crossover, &algorithm, _1, _2, _3);
    ga_obj.calculate_MO_objectives = std::bind(&MOGeneticAlgorithm::calculate_objectives, &algorithm, _1);
    ga_obj.MO_report_generation = std::bind(&MOGeneticAlgorithm::report_generation, &algorithm, _1, _2, _3);
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
    SortingStats stats;

    stats = get_classic_sorting_stats(algorithm, size, runs);

    json statsJson(stats);
    statsJson["algorithm"] = algorithm;
    std::cout << statsJson.dump(2) << std::endl;
}

void compare_algos(int size) {
    int runs = 1000;

    std::cout << "size: " << size << std::endl;

    check_classic_stats(ClassicAlgorithm::INSERTION_SORT, size, runs);
    check_classic_stats(ClassicAlgorithm::SHELL_SORT, size, runs);

    std::cout << std::endl;
}

void compare_algos(int min_size, int max_size) {
    for (int size = min_size; size <= max_size; size++) {
        compare_algos(size);
    }
}

int main(int argc, char* argv[]) {
	GaConfig config = {50, 500, 128, 0.08, 0.4 };

    try {
        run_mo_ga(config);
//        compare_algos(1000);

    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

	return 0;
}