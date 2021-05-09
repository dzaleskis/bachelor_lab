#pragma once
#include <array>
#include <iostream>
#include <fstream>
#include "openGA.hpp"
#include "json.hpp"
#include "array_utils.hpp"
#include "iterator_utils.hpp"
#include "algorithms.hpp"
#include "elements.hpp"

namespace ga_shellsort {

	using json = nlohmann::json;
	
	const int GAP_COUNT = 7;
	const int EVAL_ARRAY_SIZE = 1000;
	const int MIN_GAP_VALUE = 1;
	const int MAX_GAP_VALUE = EVAL_ARRAY_SIZE;

	bool is_gap_valid(int gap) {
		return gap >= MIN_GAP_VALUE && gap <= MAX_GAP_VALUE;
	}
	
	struct solution {
		std::array<int, GAP_COUNT> gaps; 

		void normalize() {
			std::sort(gaps.begin(), gaps.end(), std::greater<int>());
		}
	};

	struct middle_cost {
		int inversion_count;
		int comparison_count;
		int assignment_count;
	};

	struct optimization_result {
		middle_cost middle_cost;
		solution solution;
	};

	void to_json(json& j, const solution& sol) {
		j = json{
			{"gaps", sol.gaps},
		};
	}

	void from_json(const json& j, solution& sol) {
        j.at("gaps").get_to(sol.gaps);
    }

	void to_json(json& j, const middle_cost& m_cost) {
		j = json{
			{"inversion_count", m_cost.inversion_count},
			{"comparison_count", m_cost.comparison_count},
			{"assignment_count", m_cost.assignment_count},
		};
	}

	void from_json(const json& j, middle_cost& m_cost) {
        j.at("inversion_count").get_to(m_cost.inversion_count);
		j.at("comparison_count").get_to(m_cost.comparison_count);
		j.at("assignment_count").get_to(m_cost.assignment_count);
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
		array_utils::fill_sorted_descending(p.gaps, MIN_GAP_VALUE, MAX_GAP_VALUE);
	}

	bool eval_solution(const solution& p, middle_cost &c) {
		std::array<int, EVAL_ARRAY_SIZE> eval_array;
		array_utils::fill_random(eval_array);
		// sort the array using the gaps from solution
		stats stat = algorithms::shellsort_improved_reported(eval_array, p.gaps);

		c.inversion_count = array_utils::count_inversions(eval_array);
		c.assignment_count = stat.assignments;
		c.comparison_count = stat.comparisons;

		return true; // genes are accepted
	}

	solution mutate(
	const solution& X_base,
	const std::function<double(void)> &rnd01,
	double shrink_scale)
	{
		// TODO: determine need of mutating all of the gaps
		solution X_new = X_base;
		// select a random gap and mutate it
		const int selected_index = random_utils::get_random(0, X_base.gaps.size());
		const int new_gap = random_utils::get_random(MIN_GAP_VALUE, MAX_GAP_VALUE);
		X_new.gaps[selected_index] = new_gap;

		X_new.normalize();
		return X_new;
	}

	solution crossover(
		const solution& X1,
		const solution& X2,
		const std::function<double(void)> &rnd01)
	{
		solution X_new;
		// copy first solutions gaps to new solution
		std::copy(X1.gaps.begin(), X1.gaps.end(), X_new.gaps.begin());
		// copy every other gap from second solution
		iterator_utils::copy_every_other_n(X2.gaps.begin(), X2.gaps.end(), X_new.gaps.begin(), 2);

		X_new.normalize();

		return X_new;
	}

	std::vector<double> calculate_MO_objectives(const GA_Type::thisChromosomeType &X)
	{
		const int inversions = X.middle_costs.inversion_count;
		const int assignments = X.middle_costs.assignment_count;
		const int comparisons = X.middle_costs.comparison_count;
		return {
			(double)((assignments * inversions) + assignments), // prefers sequences that sort correctly at first, then optimizes
			(double)((comparisons * inversions) + comparisons), // assignments and comparisons (when inversions is 0)
		};
	}

		
	void MO_report_generation(
		int generation_number,
		const EA::GenerationType<solution, middle_cost> &last_generation,
		const std::vector<unsigned int>& pareto_front)
	{
		(void) last_generation;
		json j = {
			{"generation_number", generation_number},
			{"pareto_front", pareto_front},
		};

		std::cout << j.dump() << std::endl;
	}

	void save_results(const GA_Type &ga_obj)
	{
		std::ofstream output_file;
		output_file.open("result_shellsort.json");
		std::vector<unsigned int> paretofront_indices=ga_obj.last_generation.fronts[0];
		std::vector<optimization_result> results;
		results.resize(paretofront_indices.size());
		
		for(unsigned int i: paretofront_indices)
		{
			const auto &X = ga_obj.last_generation.chromosomes[i];
			results.push_back({X.middle_costs, X.genes});
		}

		json j = {
			{"results", results}
		};

		output_file << j << std::endl;
		output_file.close();
	}
}