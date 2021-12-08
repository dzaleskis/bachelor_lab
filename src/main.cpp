#include <iostream>
#include "json.hpp"
#include "openGA.hpp"
#include "CLI11.hpp"
#include "element.hpp"
#include "fitness.hpp"
#include "genetic_algorithm.hpp"

using json = nlohmann::json;

struct ga_config {
    int population;
    double mut_rate;
    double crossover_frac;

//    int size = 1000;
//    int gap_count = 7;
//    int evaluation_runs = 5;
};

void run_ga(const ga_config & config) {
    std::cout << "Starting genetic algorithm" << std::endl;

	EA::Chronometer timer;
	timer.tic();

	GAType ga_obj;
	ga_obj.problem_mode= EA::GA_MODE::SOGA;
	ga_obj.multi_threading=true;
	ga_obj.idle_delay_us=1; // switch between threads quickly
	ga_obj.verbose=false;
	ga_obj.population=config.population;
	// ga_obj.user_initial_solutions=initial_solutions;
	ga_obj.generation_max=200;
	ga_obj.calculate_SO_total_fitness=calculate_SO_total_fitness;
	ga_obj.init_genes=init_genes;
	ga_obj.eval_solution=eval_solution;
	ga_obj.mutate=mutate;
	ga_obj.crossover=crossover;
	ga_obj.SO_report_generation=SO_report_generation;
	ga_obj.crossover_fraction=config.crossover_frac;
	ga_obj.mutation_rate=config.mut_rate;
//	ga_obj.elite_count=75;
	ga_obj.best_stall_max=30;
	ga_obj.average_stall_max=5;
	ga_obj.solve();

	std::cout << "The problem is optimized in " << timer.toc() << " seconds." << std::endl;
	save_results(ga_obj);
}

int main(int argc, char* argv[]) {
	CLI::App app{"Running GA on gapsort"};

	ga_config config = { 50, 0.3, 0.1 };

    app.add_option("-p", config.population, "Specify GA population");
    app.add_option("-m", config.mut_rate, "Specify GA mutation rate");
    app.add_option("-x", config.crossover_frac, "Specify GA crossover fraction");

    CLI11_PARSE(app, argc, argv);

//    passes_example();
//    algorithms_example();
//    efficiency_example();
//    json_example();

    run_ga(config);

	return 0;
}