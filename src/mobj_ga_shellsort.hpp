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

namespace mobj_ga_shellsort {
	
	const int GAP_COUNT = 7;
	const int EVAL_ARRAY_SIZE = 1000;
	const int EVAL_RUNS = 1;
	const int MIN_GAP_VALUE = 1;
	const int MAX_GAP_VALUE = EVAL_ARRAY_SIZE;
	const char* result_filename = "mobj_ga_shellsort.json";

	using json = nlohmann::json;
	using mobj_solution = ga_solution::solution_base<GAP_COUNT, MIN_GAP_VALUE, MAX_GAP_VALUE>;

	struct middle_cost {
		double avg_comparison_count;
		double avg_assignment_count;
	};

	struct optimization_result {
		middle_cost cost;
        mobj_solution solution;
	};

	void to_json(json& j, const middle_cost& m_cost) {
		j = json{
			{"avg_comparison_count", m_cost.avg_comparison_count},
			{"avg_assignment_count", m_cost.avg_assignment_count},
		};
	}

	void from_json(const json& j, middle_cost& m_cost) {
		j.at("avg_comparison_count").get_to(m_cost.avg_comparison_count);
		j.at("avg_assignment_count").get_to(m_cost.avg_assignment_count);
    }

	void to_json(json& j, const optimization_result& res) {
        j = json{{"cost", res.cost}, {"my_solution", res.solution}};
    }

    void from_json(const json& j, optimization_result& res) {
        j.at("cost").get_to(res.cost);
        j.at("my_solution").get_to(res.solution);
    }

	typedef EA::Genetic<mobj_solution, middle_cost> GA_Type;
	typedef EA::GenerationType<mobj_solution, middle_cost> Generation_Type;

	void init_genes(mobj_solution& p, const std::function<double(void)> &rnd01) {
		p.init_genes(rnd01);
	}

	bool eval_solution(const mobj_solution& p, middle_cost &c) {
		auto fitness = fitness::eval_classic_fitness(p.gaps, EVAL_ARRAY_SIZE, EVAL_RUNS);

		c.avg_comparison_count = fitness.avg_comparisons;
		c.avg_assignment_count = fitness.avg_assignments;

		return true; // genes are accepted
	}

    mobj_solution mutate(
	const mobj_solution& X_base,
	const std::function<double(void)> &rnd01,
	double shrink_scale)
	{
		return X_base.mutate(rnd01, shrink_scale);
	}

    mobj_solution crossover(
		const mobj_solution& X1,
		const mobj_solution& X2,
		const std::function<double(void)> &rnd01)
	{
		return X1.crossover(X2, rnd01);
	}

	std::vector<double> calculate_MO_objectives(const GA_Type::thisChromosomeType &X)
	{
		return {
			X.middle_costs.avg_assignment_count,
			X.middle_costs.avg_comparison_count,
		};
	}

		
	void MO_report_generation(
		int generation_number,
		const EA::GenerationType<mobj_solution, middle_cost> &last_generation,
		const std::vector<unsigned int>& pareto_front)
	{
		(void) last_generation;
		json j = {
			{"generation_number", generation_number},
			{"pareto_front", pareto_front},
		};

		std::cout << j << std::endl;
	}

	void save_results(const GA_Type &ga_obj)
	{
		std::ofstream output_file;
		output_file.open(result_filename);
		std::vector<unsigned int> paretofront_indices=ga_obj.last_generation.fronts[0];
		std::vector<optimization_result> results;
		results.resize(paretofront_indices.size());
		
		for(unsigned int i: paretofront_indices)
		{
			const auto &X = ga_obj.last_generation.chromosomes[i];
			results[i] = {X.middle_costs, X.genes};
		}

		json j = {
			{"results", results}
		};

		output_file << j << std::endl;
		output_file.close();
	}
}