#pragma once
#include <array>
#include <iostream>
#include <fstream>
#include "openGA.hpp"
#include "json.hpp"
#include "array_utils.hpp"
#include "algorithms.hpp"

namespace ga_shellsort {

	using json = nlohmann::json;
	
	const int GAP_COUNT = 8;
	const int EVAL_ARRAY_SIZE = 500;
	const int MIN_GAP_VALUE = 1;
	const int MAX_GAP_VALUE = 1000;

	bool is_gap_valid(int gap) {
		return gap >= MIN_GAP_VALUE && gap <= MAX_GAP_VALUE;
	}

	struct solution {
		std::array<int, GAP_COUNT> gaps; 

		std::string to_string() const {
			json j;
			j["gaps"] = gaps;

			return j.dump();
		}

		void normalize() {
			std::sort(gaps.begin(), gaps.end(), std::greater<int>());
		}

	};

	struct middle_cost {
		int inversion_count;

		std::string to_string() const {
			json j;
			j["inversion_count"] = inversion_count;

			return j.dump();
		}
	};

	typedef EA::Genetic<solution, middle_cost> GA_Type;
	typedef EA::GenerationType<solution, middle_cost> Generation_Type;

	void init_genes(solution& p, const std::function<double(void)> &rnd01) {
		array_utils::fill_sorted_descending(p.gaps, MIN_GAP_VALUE, MAX_GAP_VALUE);
	}

	bool eval_solution(const solution& p, middle_cost &c) {
		std::array<int, EVAL_ARRAY_SIZE> eval_array;
		array_utils::fill_random(eval_array);
		// sort the array using the gaps from solution
		algorithms::shellsort_improved(eval_array, p.gaps);
		// number of inversions is the fitness of gap sequence
		// 0 being a sorted array
		c.inversion_count = array_utils::count_inversions(eval_array);
		return true; // genes are accepted
	}

	solution mutate(
	const solution& X_base,
	const std::function<double(void)> &rnd01,
	double shrink_scale)
	{
		solution X_new = X_base;
		const double mut = 0.2 * shrink_scale; // mutation radius

		for (int i = 0; i < X_new.gaps.size(); i++) {
			const int prev_gap = X_new.gaps[i];
			int mutated_gap;
			do {
				mutated_gap = prev_gap + (int)std::round(mut * (rnd01() - rnd01()));
			} while(!is_gap_valid(mutated_gap));

			X_new.gaps[i] = mutated_gap;
		}

		X_new.normalize();
		return X_new;
	}

	solution crossover(
		const solution& X1,
		const solution& X2,
		const std::function<double(void)> &rnd01)
	{
		solution X_new;
		// copy second solutions gaps to new solution
		X_new.gaps = X2.gaps;
		// copy the first arrays first half
		auto distance = std::distance(X1.gaps.begin(), X1.gaps.end());
		auto iterator = X1.gaps.begin();
		std::advance(iterator, distance / 2);

		std::copy(X1.gaps.begin(), iterator, X_new.gaps.begin());
		X_new.normalize();

		return X_new;
	}

	std::vector<double> calculate_MO_objectives(const GA_Type::thisChromosomeType &X)
	{
		return {
			(double)X.middle_costs.inversion_count,
			0
		};
	}

		
	void MO_report_generation(
		int generation_number,
		const EA::GenerationType<solution, middle_cost> &last_generation,
		const std::vector<unsigned int>& pareto_front)
	{
		(void) last_generation;
		json j;
		j["generation_number"] = generation_number;
		j["pareto_front"] = pareto_front;
		std::cout << j.dump() << std::endl;
	}

	void save_results(const GA_Type &ga_obj)
	{
		std::ofstream output_file;
		output_file.open("result_shellsort.txt");
		std::vector<unsigned int> paretofront_indices=ga_obj.last_generation.fronts[0];
		for(unsigned int i:paretofront_indices)
		{
			const auto &X = ga_obj.last_generation.chromosomes[i];
			output_file
				<<i<<"\t"
				<<X.genes.to_string()<<"\t"
				<<X.middle_costs.to_string()<< std::endl;
		}
		output_file.close();
	}
}