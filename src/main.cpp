#include <iostream>
#include "json.hpp"
#include "openGA.hpp"
#include "CLI11.hpp"
#include "element.hpp"
#include "fitness.hpp"
#include "pass.hpp"

using json = nlohmann::json;

struct ga_config {
    int population;
    double mut_rate;
    double crossover_frac;
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

void elements_example() {
    auto report = global_measure.with_report([&] () {
        auto e1 = Element<int>(5);
        auto e2 = Element<int>(10);

        assert(e1 < e2);
        e1 = e2;
        assert(e1 == e2);
    });

    std::cout << "assignments performed: " << report.assignments << '\n';
    std::cout << "comparisons performed: " << report.comparisons << '\n';
}

void passes_example() {
    auto e1 = Element<int>(15);
    auto e2 = Element<int>(10);

    std::vector<Element<int>> vec = {e1, e2};

    auto factory = PassFactory<typeof(vec)>();
    auto pass = factory.getPass(PassType::INSERTION);
    pass->perform_pass(vec, vec.size(), 1);

    for (auto e: vec) {
        std::cout << e.get_value() << '\n';
    }
}


int main(int argc, char* argv[]) {
	CLI::App app{"Running GA on gapsort"};

	ga_config config = { 1000, 0.02, 0.1 };

    app.add_option("-p", config.population, "Specify GA population");
    app.add_option("-m", config.mut_rate, "Specify GA mutation rate");
    app.add_option("-x", config.crossover_frac, "Specify GA crossover fraction");

    CLI11_PARSE(app, argc, argv);

    elements_example();
//    passes_example();
//    run_ga(config);

	return 0;
}