#include <iostream>
#include <functional>
#include "json.hpp"
#include "openGA.hpp"
#include "CLI11.hpp"
#include "genetic_algorithm.hpp"
#include "custom_algorithms.hpp"

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
    std::vector<SortingStats> all_stats;

    for (int i = 0; i < runs; i++) {
        auto stats = get_classic_sorting_stats(algorithm, size);
        all_stats.emplace_back(stats);
    }

    for (const auto& stats: all_stats) {
        json statsJson(stats);
        std::cout << statsJson.dump(2) << std::endl;
    }
}

void check_custom_stats(CustomAlgorithm algorithm, int size, int runs) {
    std::vector<SortingStats> all_stats;

    for (int i = 0; i < runs; i++) {
        auto stats = get_custom_sorting_stats(algorithm, size);
        all_stats.emplace_back(stats);
    }

    for (const auto& stats: all_stats) {
        json statsJson(stats);
        std::cout << statsJson.dump(2) << std::endl;
    }
}

int main(int argc, char* argv[]) {
	CLI::App app{"Running GA for sorting algorithm construction"};

	GaConfig config = {100, 500, 128, 0.08, 0.4 };

    app.add_option("-p", config.population, "Specify GA population");
    app.add_option("-s", config.size, "Specify size of data to sort");
    app.add_option("-m", config.mut_rate, "Specify GA mutation rate");
    app.add_option("-x", config.crossover_frac, "Specify GA crossover fraction");

    CLI11_PARSE(app, argc, argv);

    try {
        run_mo_ga(config);
//        check_classic_stats(ClassicAlgorithm::INSERTION_SORT_IMPROVED, 128, 50);
//        check_classic_stats(ClassicAlgorithm::SHELLSORT_IMPROVED, 128, 50);
//        check_custom_stats(CustomAlgorithm::ONE_TWO_SORT, 64, 100);

    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

	return 0;
}