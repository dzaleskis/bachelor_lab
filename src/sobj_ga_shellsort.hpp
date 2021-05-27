#pragma once
#include <array>
#include <iostream>
#include <fstream>
#include "openGA.hpp"
#include "json.hpp"
#include "array_utils.hpp"
#include "iterator_utils.hpp"
#include "algorithms.hpp"
#include "fitness.hpp"
#include "solution.hpp"

namespace sobj_ga_shellsort {
	
	const int GAP_COUNT = 15;
	const int EVAL_ARRAY_SIZE = 1000000;
	const int EVAL_RUNS = 3;
	const char* result_filename = "sobj_ga_shellsort.json";

	using json = nlohmann::json;
	using sobj_solution = ga_solution::solution_extended<GAP_COUNT>;

	struct middle_cost {
		double avg_comparison_count;
	};

	struct optimization_result {
		middle_cost cost;
		sobj_solution solution;
	};

	void to_json(json& j, const middle_cost& m_cost) {
		j = json{
			{"avg_comparison_count", m_cost.avg_comparison_count}
		};
	}

	void from_json(const json& j, middle_cost& m_cost) {
		j.at("avg_comparison_count").get_to(m_cost.avg_comparison_count);
    }

	void to_json(json& j, const optimization_result& res) {
        j = json{{"cost", res.cost}, {"sobj_solution", res.solution}};
    }

    void from_json(const json& j, optimization_result& res) {
        j.at("cost").get_to(res.cost);
        j.at("sobj_solution").get_to(res.solution);
    }

	typedef EA::Genetic<sobj_solution, middle_cost> GA_Type;
	typedef EA::GenerationType<sobj_solution, middle_cost> Generation_Type;

	void init_genes(sobj_solution& p, const std::function<double(void)> &rnd01) {
		p.init_genes(rnd01);
	}

	bool eval_solution(const sobj_solution& p, middle_cost &c) {
		auto fitness = fitness::eval_classic_fitness(p.gaps, EVAL_ARRAY_SIZE, EVAL_RUNS);

		c.avg_comparison_count = fitness.avg_comparisons;

		return true; // genes are accepted
	}

	sobj_solution mutate(
	const sobj_solution& X_base,
	const std::function<double(void)> &rnd01,
	double shrink_scale)
	{
		return X_base.mutate(rnd01, shrink_scale);
	}

	sobj_solution crossover(
		const sobj_solution& X1,
		const sobj_solution& X2,
		const std::function<double(void)> &rnd01)
	{
		return X1.crossover(X2, rnd01);
	}

    double calculate_SO_total_fitness(const GA_Type::thisChromosomeType &X)
    {
        // finalize the cost
        return X.middle_costs.avg_comparison_count;
    }

    void SO_report_generation(
	int generation_number,
	const EA::GenerationType<sobj_solution, middle_cost> &last_generation,
	const sobj_solution& best_genes)
    {
        json j = {
			{"generation_number", generation_number},
			{"best_genes", best_genes},
            {"best_total_cost", last_generation.best_total_cost},
            {"best_average_cost", last_generation.average_cost},
		};

		std::cout << j << std::endl;
    }

	void save_results(const GA_Type &ga_obj)
	{
		std::ofstream output_file;
		output_file.open(result_filename);
        int best_index = ga_obj.last_generation.best_chromosome_index;
        auto best_solution = ga_obj.last_generation.chromosomes[best_index];
		std::vector<optimization_result> results;
		results.push_back({best_solution.middle_costs, best_solution.genes});

		json j = {
			{"results", results}
		};

		output_file << j << std::endl;
		output_file.close();
	}
}