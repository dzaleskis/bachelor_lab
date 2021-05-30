#include <iostream>
#include <array>
#include "json.hpp"
#include "array_utils.hpp"
#include "openGA.hpp"
#include "sobj_ga_shellsort.hpp"
#include "CLI11.hpp"
#include "benchmarks.hpp"

using json = nlohmann::json;

struct ga_config {
    int population;
    double mut_rate;
    double crossover_frac;
};

void run_classic_bench(std::vector<std::vector<int>> & gaps, int array_size, int runs) {
    std::cout << std::endl << "ops bench" << std::endl;
    benchmarks::bench_classic_ops(gaps, array_size, runs);
    std::cout << std::endl << "time bench" << std::endl;
    // benchmarks::bench_classic_time(gaps, array_size, runs);
}

void run_improved_bench(std::vector<std::vector<int>> & gaps, int array_size, int runs) {
    std::cout << std::endl << "ops bench" << std::endl;
    benchmarks::bench_improved_ops(gaps, array_size, runs);
    std::cout << std::endl << "time bench" << std::endl;
    benchmarks::bench_improved_time(gaps, array_size, runs);
}

void run_sobj_ga_shellsort(const std::vector<std::vector<int>> & initial_gaps, const ga_config & config) {

	const int GAP_COUNT = sobj_ga_shellsort::GAP_COUNT;
	const int MAX_GAP_VALUE = sobj_ga_shellsort::MAX_GAP_VALUE;
    std::vector<ga_solution::solution_extended<GAP_COUNT, MAX_GAP_VALUE>> initial_solutions;

    for (const auto& gaps: initial_gaps) {
        initial_solutions.emplace_back(gaps);
    }

	EA::Chronometer timer;
	timer.tic();

	sobj_ga_shellsort::GA_Type ga_obj;
	ga_obj.problem_mode= EA::GA_MODE::SOGA;
	ga_obj.multi_threading=true;
	ga_obj.idle_delay_us=1; // switch between threads quickly
	ga_obj.verbose=false;
	ga_obj.population=config.population;
	ga_obj.user_initial_solutions=initial_solutions;
	ga_obj.generation_max=200;
	ga_obj.calculate_SO_total_fitness=sobj_ga_shellsort::calculate_SO_total_fitness;
	ga_obj.init_genes=sobj_ga_shellsort::init_genes;
	ga_obj.eval_solution=sobj_ga_shellsort::eval_solution;
	ga_obj.mutate= sobj_ga_shellsort::mutate;
	ga_obj.crossover=sobj_ga_shellsort::crossover;
	ga_obj.SO_report_generation=sobj_ga_shellsort::SO_report_generation;
	ga_obj.crossover_fraction=config.crossover_frac;
	ga_obj.mutation_rate=config.mut_rate;
	ga_obj.elite_count=75;
	ga_obj.best_stall_max=30;
	ga_obj.average_stall_max=5;
	ga_obj.verbose=false;
	ga_obj.solve();

	std::cout<<"The problem is optimized in "<<timer.toc()<<" seconds."<<std::endl;
	sobj_ga_shellsort::save_results(ga_obj);
}

int main(int argc, char* argv[]) {
	CLI::App app{"Running GA on shellsort"};

    std::string filename;
	std::string mode = "sobj";
	int runs = 100;
	int array_size = 1000;
	ga_config config = { 1000, 0.02, 0.1 };

    app.add_option("-f,--file", filename, "Specify input file for gap initialization");
	app.add_option("-m,--mode", mode, "Specify run mode");
	app.add_option("-r,--runs", runs, "Specify benchmark runs");
    app.add_option("-s,--size", array_size, "Specify benchmark array size");
    app.add_option("-p,--pop", config.population, "Specify GA population");
    app.add_option("-x,--x_rate", config.mut_rate, "Specify GA mutation rate");
    app.add_option("-c,--cro", config.crossover_frac, "Specify GA crossover fraction");

    CLI11_PARSE(app, argc, argv);

	std::vector<std::vector<int>> initial_gaps;

	if (!filename.empty()) {
		std::ifstream input_file;
		input_file.open(filename);
		json j;
		input_file >> j;
		initial_gaps = j.get<std::vector<std::vector<int>>>();
		std::cout << initial_gaps.size() << " gap sequences parsed from file" << std::endl;
	}

    if (mode == "sobj") {
		run_sobj_ga_shellsort(initial_gaps, config);
	} else if (mode == "classic_bench") {
        run_classic_bench(initial_gaps, array_size, runs);
    } else if (mode == "improved_bench") {
        run_improved_bench(initial_gaps, array_size, runs);
    } else {
		std::cerr << "invalid mode" << std::endl;
		exit(-1);
	}

	return 0;
}