#include <iostream>
#include "json.hpp"
#include "openGA.hpp"
#include "CLI11.hpp"
#include "element.hpp"
#include "fitness.hpp"
#include "pass.hpp"
#include "sorting_algorithm.hpp"
#include "fitness.hpp"
#include "genetic.hpp"
#include "json_serializers.hpp"

using json = nlohmann::json;
using Solution = genetic::Solution;

struct ga_config {
    int population;
    double mut_rate;
    double crossover_frac;

    int size = 1000;
//    int gap_count = 7;
    int evaluation_runs = 5;
};

void run_ga(const ga_config & config) {
	EA::Chronometer timer;
	timer.tic();

//	sobj_ga_shellsort::GA_Type ga_obj;
//	ga_obj.problem_mode= EA::GA_MODE::SOGA;
//	ga_obj.multi_threading=true;
//	ga_obj.idle_delay_us=1; // switch between threads quickly
//	ga_obj.verbose=false;
//	ga_obj.population=config.population;
//	// ga_obj.user_initial_solutions=initial_solutions;
//	ga_obj.generation_max=200;
//	ga_obj.calculate_SO_total_fitness=sobj_ga_shellsort::calculate_SO_total_fitness;
//	ga_obj.init_genes=sobj_ga_shellsort::init_genes;
//	ga_obj.eval_solution=sobj_ga_shellsort::eval_solution;
//	ga_obj.mutate= sobj_ga_shellsort::mutate;
//	ga_obj.crossover=sobj_ga_shellsort::crossover;
//	ga_obj.SO_report_generation=sobj_ga_shellsort::SO_report_generation;
//	ga_obj.crossover_fraction=config.crossover_frac;
//	ga_obj.mutation_rate=config.mut_rate;
//	ga_obj.elite_count=75;
//	ga_obj.best_stall_max=30;
//	ga_obj.average_stall_max=5;
//	ga_obj.verbose=false;
//	ga_obj.solve();

	std::cout<<"The problem is optimized in "<<timer.toc()<<" seconds."<<std::endl;
//	sobj_ga_shellsort::save_results(ga_obj);
}

void passes_example() {
    std::vector<int> vec(200);
    iterator_utils::fill_random(vec.begin(), vec.end());

    auto pass = PassFactory::getPass<typeof(vec)>(PassType::INSERTION);
    pass->performPass(vec, vec.size(), 1);

    assert(std::is_sorted(vec.begin(), vec.end()));
}

void algorithms_example() {
    std::vector<int> vec(200);
    iterator_utils::fill_random(vec.begin(), vec.end());

    auto blueprint = AlgorithmBlueprint({ PassBlueprint(PassType::INSERTION, 1) });
    auto algorithm = ConcreteAlgorithmFactory::getConcreteAlgorithm<typeof(vec)>(blueprint);

    algorithm->sort(vec);

    assert(std::is_sorted(vec.begin(), vec.end()));
}

void efficiency_example() {
    auto blueprint = AlgorithmBlueprint({ PassBlueprint(PassType::BRICK, 1) });
    auto stats = fitness::get_sorting_stats(blueprint, 5);
    auto efficiency = fitness::calculate_fitness(stats);

    std::cout << "inversions " << stats.inversions << " comparisons " << stats.comparisons << " assignments " << stats.assignments << "\n";
    std::cout << "efficiency is: " << efficiency << "\n";
}

void json_example() {
    auto blueprint = AlgorithmBlueprint({ PassBlueprint(PassType::BRICK, 1) });
    json serialized(blueprint);

    auto deserialized = serialized.get<AlgorithmBlueprint>();
    json serializedAgain(deserialized);

    std::cout << serialized.dump() << "\n";
    std::cout << serializedAgain.dump() << "\n";
}

int main(int argc, char* argv[]) {
	CLI::App app{"Running GA on gapsort"};

	ga_config config = { 1000, 0.02, 0.1 };

    app.add_option("-p", config.population, "Specify GA population");
    app.add_option("-m", config.mut_rate, "Specify GA mutation rate");
    app.add_option("-x", config.crossover_frac, "Specify GA crossover fraction");

    CLI11_PARSE(app, argc, argv);

//    passes_example();
//    algorithms_example();
//    efficiency_example();
    json_example();

//    run_ga(config);

	return 0;
}