#include <iostream>
#include <array>
#include <algorithm>
#include "json.hpp"
#include "array_utils.hpp"
#include "algorithms.hpp"
#include "openGA.hpp"
#include "mobj_ga_shellsort.hpp"
#include "sobj_ga_shellsort.hpp"
#include "CLI11.hpp"
#include "solution.hpp"

using json = nlohmann::json;

void run_mobj_ga_shellsort(std::vector<std::vector<int>> & initial_gaps) {

	const int GAP_COUNT = mobj_ga_shellsort::GAP_COUNT;
	const int MIN_GAP_VALUE = mobj_ga_shellsort::MIN_GAP_VALUE;
	const int MAX_GAP_VALUE = mobj_ga_shellsort::MAX_GAP_VALUE;

	std::vector<ga_solution::solution_base<GAP_COUNT, MIN_GAP_VALUE, MAX_GAP_VALUE>> initial_solutions;

	for (auto gaps: initial_gaps) {
		initial_solutions.push_back(ga_solution::solution_base<GAP_COUNT, MIN_GAP_VALUE, MAX_GAP_VALUE>(gaps));
	}

	EA::Chronometer timer;
	timer.tic();

	mobj_ga_shellsort::GA_Type ga_obj;
	ga_obj.problem_mode= EA::GA_MODE::NSGA_III;
	ga_obj.multi_threading=true;
	ga_obj.idle_delay_us=1; // switch between threads quickly
	ga_obj.verbose=false;
	ga_obj.population=500;
	ga_obj.user_initial_solutions=initial_solutions;
	ga_obj.generation_max=20;
	ga_obj.calculate_MO_objectives=mobj_ga_shellsort::calculate_MO_objectives;
	ga_obj.init_genes=mobj_ga_shellsort::init_genes;
	ga_obj.eval_solution=mobj_ga_shellsort::eval_solution;
	ga_obj.mutate= mobj_ga_shellsort::mutate;
	ga_obj.crossover= mobj_ga_shellsort::crossover;
	ga_obj.MO_report_generation=mobj_ga_shellsort::MO_report_generation;
	ga_obj.crossover_fraction=0.5;
	ga_obj.mutation_rate=0.5;
	ga_obj.solve();

	std::cout<<"The problem is optimized in "<<timer.toc()<<" seconds."<<std::endl;
	mobj_ga_shellsort::save_results(ga_obj);
}


void run_sobj_ga_shellsort(std::vector<std::vector<int>> & initial_gaps) {

	const int GAP_COUNT = sobj_ga_shellsort::GAP_COUNT;
	const int MIN_GAP_VALUE = sobj_ga_shellsort::MIN_GAP_VALUE;
	const int MAX_GAP_VALUE = sobj_ga_shellsort::MAX_GAP_VALUE;

	std::vector<ga_solution::solution_base<GAP_COUNT, MIN_GAP_VALUE, MAX_GAP_VALUE>> initial_solutions;

	for (auto gaps: initial_gaps) {
		initial_solutions.push_back(ga_solution::solution_base<GAP_COUNT, MIN_GAP_VALUE, MAX_GAP_VALUE>(gaps));
	}

	EA::Chronometer timer;
	timer.tic();

	sobj_ga_shellsort::GA_Type ga_obj;
	ga_obj.problem_mode= EA::GA_MODE::SOGA;
	ga_obj.multi_threading=true;
	ga_obj.idle_delay_us=1; // switch between threads quickly
	ga_obj.verbose=false;
	ga_obj.population=10000;
	ga_obj.user_initial_solutions=initial_solutions;
	ga_obj.generation_max=100;
	ga_obj.calculate_SO_total_fitness=sobj_ga_shellsort::calculate_SO_total_fitness;
	ga_obj.init_genes=sobj_ga_shellsort::init_genes;
	ga_obj.eval_solution=sobj_ga_shellsort::eval_solution;
	ga_obj.mutate= sobj_ga_shellsort::mutate;
	ga_obj.crossover=sobj_ga_shellsort::crossover;
	ga_obj.SO_report_generation=sobj_ga_shellsort::SO_report_generation;
	ga_obj.crossover_fraction=0.5;
	ga_obj.mutation_rate=0.8;
	ga_obj.best_stall_max=20;
	ga_obj.average_stall_max=20;
	ga_obj.solve();

	std::cout<<"The problem is optimized in "<<timer.toc()<<" seconds."<<std::endl;
	sobj_ga_shellsort::save_results(ga_obj);

	auto ciura_gaps = std::array<int, 8> {701, 301, 132, 57, 23, 10, 4, 1};
	auto ciura_stats = benchmarks::measure_ops_int<1000>(ciura_gaps);

	auto my_gaps = ga_obj.last_generation.chromosomes[ga_obj.last_generation.best_chromosome_index].genes.gaps;
	auto my_stats = benchmarks::measure_ops_int<1000>(my_gaps);

	std::cout << "ciura" << ciura_stats.classic_assignments << std::endl;
	std::cout << "mine" << my_stats.classic_assignments << std::endl;

}

int main(int argc, char* argv[]) {
	CLI::App app{"Running GA on shellsort"};

    std::string filename = "";
	std::string mode = "mobj";
    app.add_option("-f,--file", filename, "Specify input file for gap initialization");
	app.add_option("-m,--mode", mode, "Specify run mode");

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

	if (mode == "mobj") {
		run_mobj_ga_shellsort(initial_gaps);
	} else if (mode == "sobj") {
		run_sobj_ga_shellsort(initial_gaps);
	} else {
		std::cerr << "invalid mode" << std::endl;
		exit(-1);
	}

	return 0;
}