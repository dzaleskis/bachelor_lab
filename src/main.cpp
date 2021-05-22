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
	ga_obj.population=10000;
	ga_obj.user_initial_solutions=initial_solutions;
	ga_obj.generation_max=100;
	ga_obj.calculate_MO_objectives=mobj_ga_shellsort::calculate_MO_objectives;
	ga_obj.init_genes=mobj_ga_shellsort::init_genes;
	ga_obj.eval_solution=mobj_ga_shellsort::eval_solution;
	ga_obj.mutate= mobj_ga_shellsort::mutate;
	ga_obj.crossover= mobj_ga_shellsort::crossover;
	ga_obj.MO_report_generation=mobj_ga_shellsort::MO_report_generation;
	ga_obj.crossover_fraction=0.1;
	ga_obj.mutation_rate=0.8;
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
	ga_obj.population=15000;
	ga_obj.user_initial_solutions=initial_solutions;
	ga_obj.generation_max=500;
	ga_obj.calculate_SO_total_fitness=sobj_ga_shellsort::calculate_SO_total_fitness;
	ga_obj.init_genes=sobj_ga_shellsort::init_genes;
	ga_obj.eval_solution=sobj_ga_shellsort::eval_solution;
	ga_obj.mutate= sobj_ga_shellsort::mutate;
	ga_obj.crossover=sobj_ga_shellsort::crossover;
	ga_obj.SO_report_generation=sobj_ga_shellsort::SO_report_generation;
	ga_obj.crossover_fraction=0.01;
	ga_obj.mutation_rate=0.9;
	ga_obj.best_stall_max=20;
	ga_obj.average_stall_max=20;
	ga_obj.solve();

	std::cout<<"The problem is optimized in "<<timer.toc()<<" seconds."<<std::endl;
	sobj_ga_shellsort::save_results(ga_obj);

	auto ciura_gaps = std::array<int, 7> {301, 132, 57, 23, 10, 4, 1};
	auto ciura_stats = fitness::eval_classic_fitness<1000>(ciura_gaps, 20);

	auto simpson_gaps = std::array<int, 7> {893, 219, 83, 36, 13, 4, 1};
	auto simpson_stats = fitness::eval_classic_fitness<1000>(simpson_gaps, 20);

	auto my_gaps = ga_obj.last_generation.chromosomes[ga_obj.last_generation.best_chromosome_index].genes.gaps;
	auto my_stats = fitness::eval_classic_fitness<1000>(my_gaps, 20);

	std::cout << "ciura assignments:" << ciura_stats.avg_assignments << " comparisons: " << ciura_stats.avg_comparisons << std::endl;
	std::cout << "simpson assignments:" << simpson_stats.avg_assignments << " comparisons: " << simpson_stats.avg_comparisons << std::endl;
	std::cout << "my assignments:" << my_stats.avg_assignments << " comparisons: " << my_stats.avg_comparisons << std::endl;
}

void print_gap_seq(std::vector<int> gap_seq) {
	for (const auto gap: gap_seq) {
		std::cout << gap << ' ';
	}

	std::cout << std::endl;
}

void run_bench(std::vector<std::vector<int>> & gaps, int runs) {
	const int GAP_SEQ_LEN = 7;

	int best_comparisons = INT_MAX;
	int best_assignments = INT_MAX;

	std::vector<int> best_assignments_seq;
	std::vector<int> best_comparisons_seq;

	best_comparisons_seq.reserve(GAP_SEQ_LEN);
	best_assignments_seq.reserve(GAP_SEQ_LEN);

	for (auto gap_seq: gaps) {
		print_gap_seq(gap_seq);
		
		std::array<int, GAP_SEQ_LEN> gap_arr;
		std::copy_n(gap_seq.begin(), GAP_SEQ_LEN, gap_arr.begin());

		auto stats = fitness::eval_classic_fitness<1000>(gap_arr, runs);
		std::cout << "assignments:" << stats.avg_assignments << " comparisons: " << stats.avg_comparisons << std::endl;

		if (stats.avg_assignments < best_assignments) {
			best_assignments = stats.avg_assignments;
			best_assignments_seq = gap_seq;
		}

		if (stats.avg_comparisons < best_comparisons) {
			best_comparisons = stats.avg_comparisons;
			best_comparisons_seq = gap_seq;
		}
	}

	std::cout << std::endl;
	std::cout << "best assignments:" << best_assignments << std::endl;
	print_gap_seq(best_assignments_seq);

	std::cout << std::endl;
	std::cout << " best comparisons: " << best_comparisons << std::endl;
	print_gap_seq(best_comparisons_seq);
}

int main(int argc, char* argv[]) {
	CLI::App app{"Running GA on shellsort"};

    std::string filename = "";
	std::string mode = "mobj";
	int runs = 100;

    app.add_option("-f,--file", filename, "Specify input file for gap initialization");
	app.add_option("-m,--mode", mode, "Specify run mode");
	app.add_option("-r,--runs", runs, "Specify benchmark runs");

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
	} else if (mode == "bench") {
		run_bench(initial_gaps, runs);
	} else {
		std::cerr << "invalid mode" << std::endl;
		exit(-1);
	}

	return 0;
}