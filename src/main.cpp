#include <iostream>
#include "json.hpp"
#include "openGA.hpp"
#include "CLI11.hpp"
#include "stats.hpp"
#include "genetic_algorithm.hpp"

using json = nlohmann::json;

struct GaConfig {
    int population;
    int size;
    int eval_runs;
    double mut_rate;
    double crossover_frac;
};

void run_ga(const GaConfig & config) {
    std::cout << "Starting genetic algorithm" << std::endl;

	EA::Chronometer timer;
	timer.tic();

    MOGeneticAlgorithm algorithm(config.size, config.eval_runs);

	GAType ga_obj;
	ga_obj.problem_mode=EA::GA_MODE::NSGA_III;
	ga_obj.multi_threading=true;
	ga_obj.idle_delay_us=1;
	ga_obj.verbose=false;
	ga_obj.population=config.population;
	ga_obj.generation_max=250;
	ga_obj.calculate_MO_objectives=[&](const GAType::thisChromosomeType &x) { return algorithm.calculate_objectives(x);};
	ga_obj.init_genes=[&](Solution& s, const std::function<double(void)> &rnd01) { return algorithm.init_genes(s, rnd01);};
	ga_obj.eval_solution=[&](const Solution& s, MiddleCost &c) { return algorithm.eval_solution(s, c);};
	ga_obj.mutate=[&](const Solution& s_base, const std::function<double(void)> &rnd01, double shrink_scale) { return algorithm.mutate(s_base, rnd01, shrink_scale);};
	ga_obj.crossover=[&](const Solution& s1, const Solution& s2, const std::function<double(void)> &rnd01) { return algorithm.crossover(s1, s2, rnd01);};
	ga_obj.MO_report_generation=[&](int gen, const EA::GenerationType<Solution,MiddleCost> &last_gen, const std::vector<unsigned int>& p_front) { return algorithm.report_generation(gen, last_gen, p_front);};
	ga_obj.crossover_fraction=config.crossover_frac;
	ga_obj.mutation_rate=config.mut_rate;
	ga_obj.elite_count=5;
	ga_obj.best_stall_max=50;
	ga_obj.average_stall_max=10;
	ga_obj.solve();

	std::cout << "The problem is optimized in " << timer.toc() << " seconds." << std::endl;
	algorithm.save_results(ga_obj);
}

int main(int argc, char* argv[]) {
	CLI::App app{"Running GA for sorting algorithm construction"};

	GaConfig config = {100, 100, 1, 0.05, 0.5 };

    app.add_option("-p", config.population, "Specify GA population");
    app.add_option("-s", config.size, "Specify size of data to sort");
    app.add_option("-e", config.eval_runs, "Specify amount of runs for evaluation");
    app.add_option("-m", config.mut_rate, "Specify GA mutation rate");
    app.add_option("-x", config.crossover_frac, "Specify GA crossover fraction");

    CLI11_PARSE(app, argc, argv);

    try {
        run_ga(config);   
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

	return 0;
}