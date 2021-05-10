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
	
	const int GAP_COUNT = 7;
	const int EVAL_ARRAY_SIZE = 1000;
	const int EVAL_RUNS = 10;
	const int MIN_GAP_VALUE = 1;
	const int MAX_GAP_VALUE = EVAL_ARRAY_SIZE;
	const char* result_filename = "sobj_ga_shellsort.json";

	using json = nlohmann::json;
	using solution = ga_solution::solution_base<GAP_COUNT, MIN_GAP_VALUE, MAX_GAP_VALUE>;

	struct middle_cost {
		double avg_comparison_count;
	};

	struct optimization_result {
		middle_cost middle_cost;
		solution solution;
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
        j = json{{"middle_cost", res.middle_cost}, {"solution", res.solution}};
    }

    void from_json(const json& j, optimization_result& res) {
        j.at("middle_cost").get_to(res.middle_cost);
        j.at("solution").get_to(res.solution);
    }

	typedef EA::Genetic<solution, middle_cost> GA_Type;
	typedef EA::GenerationType<solution, middle_cost> Generation_Type;

	void init_genes(solution& p, const std::function<double(void)> &rnd01) {
		p.init_genes(rnd01);
	}

	bool eval_solution(const solution& p, middle_cost &c) {
		auto fitness = fitness::eval_classic_fitness<EVAL_ARRAY_SIZE>(p.gaps, EVAL_RUNS);

		c.avg_comparison_count = fitness.avg_comparisons;

		return true; // genes are accepted
	}

	solution mutate(
	const solution& X_base,
	const std::function<double(void)> &rnd01,
	double shrink_scale)
	{
		return X_base.mutate(rnd01, shrink_scale);
	}

	solution crossover(
		const solution& X1,
		const solution& X2,
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
	const EA::GenerationType<solution, middle_cost> &last_generation,
	const solution& best_genes)
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