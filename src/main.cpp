#include <iostream>
#include <array>
#include <algorithm>

#include "array_utils.hpp"
#include "algorithms.hpp"
#include "openGA.hpp"
#include "ga_shellsort.hpp"

int main(int argc, char* argv[]) {
    EA::Chronometer timer;
	timer.tic();

	ga_shellsort::GA_Type ga_obj;
	ga_obj.problem_mode= EA::GA_MODE::NSGA_III;
	ga_obj.multi_threading=true;
	ga_obj.idle_delay_us=1; // switch between threads quickly
	ga_obj.verbose=false;
	ga_obj.population=500;
	ga_obj.generation_max=20;
	ga_obj.calculate_MO_objectives=ga_shellsort::calculate_MO_objectives;
	ga_obj.init_genes=ga_shellsort::init_genes;
	ga_obj.eval_solution=ga_shellsort::eval_solution;
	ga_obj.mutate= ga_shellsort::mutate;
	ga_obj.crossover= ga_shellsort::crossover;
	ga_obj.MO_report_generation=ga_shellsort::MO_report_generation;
	ga_obj.crossover_fraction=0.7;
	ga_obj.mutation_rate=0.4;
	ga_obj.solve();

	std::cout<<"The problem is optimized in "<<timer.toc()<<" seconds."<<std::endl;
	ga_shellsort::save_results(ga_obj);

	return 0;
}