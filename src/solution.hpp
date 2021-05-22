#pragma once
#include <array>
#include <functional>
#include "json.hpp"
#include "iterator_utils.hpp"

namespace ga_solution {

    using json = nlohmann::json;

	const double EQUAL_PROBABILITY = 0.5;
	const double MUT_SCALE = 0.2;
	const double MUTATION_PROBABILITY = 0.5;
	const int MUT_CEILING = 100;

    template <std::size_t GAP_COUNT, int MIN_GAP_VALUE, int MAX_GAP_VALUE>
	struct solution_base {
		std::vector<int> gaps = std::vector<int>(GAP_COUNT);

        solution_base() = default;

        explicit solution_base(std::vector<int> x) {
            iterator_utils::fill_random(gaps.begin(), gaps.end(), MIN_GAP_VALUE, MAX_GAP_VALUE);
            std::copy_backward(x.begin(), x.end(), gaps.end());
            normalize();
        }

		void init_genes(const std::function<double(void)> &rnd01) {
			for (int i = 0; i < gaps.size(); i++) {
				gaps[i] = std::floor(rnd01() * MAX_GAP_VALUE);
			}
			normalize();
		}

		void normalize() {
			std::sort(gaps.begin(), gaps.end(), std::greater<int>());
			// hack for correct sequences
			gaps[gaps.size() - 1] = 1;
		}

		solution_base crossover(
		const solution_base& X2,
		const std::function<double(void)> &rnd01) const {
			solution_base X_new;

			// using uniform distribution
			for (int i = 0; i < X_new.gaps.size(); i++) {
				double prob = rnd01();
				if (prob < EQUAL_PROBABILITY) {
					X_new.gaps[i] = this->gaps[i];
				} else {
					X_new.gaps[i] = X2.gaps[i];
				}
			}

			X_new.normalize();

			return X_new;
		}

		solution_base mutate(
		const std::function<double(void)> &rnd01,
		double shrink_scale) const {
			solution_base X_new = *this;
			const double mut_radius = MUT_SCALE * shrink_scale;

			for (int i = 0; i < gaps.size(); i++) {
				bool do_mutate = rnd01() < MUTATION_PROBABILITY;
				if (do_mutate) {
					int new_gap;
					do {
						double raw_mutation = mut_radius * (rnd01()-rnd01()); // value is between -1.0 and 1.0
						int adjusted_mutation = std::floor(raw_mutation * MUT_CEILING); // scale the value
						new_gap = gaps[i] + adjusted_mutation;
					} while(!is_gap_valid(new_gap));
					X_new.gaps[i] = new_gap;
				}
			}

			X_new.normalize();

			return X_new;
		}

		inline bool is_gap_valid(int gap) const {
			return gap >= MIN_GAP_VALUE && gap <= MAX_GAP_VALUE;
		}
	};

    template <std::size_t GAP_COUNT, int MIN_GAP_VALUE, int MAX_GAP_VALUE>
    void to_json(json& j, const solution_base<GAP_COUNT, MIN_GAP_VALUE, MAX_GAP_VALUE>& sol) {
		j = json{
			{"gaps", sol.gaps},
		};
	}

    template <std::size_t GAP_COUNT, int MIN_GAP_VALUE, int MAX_GAP_VALUE>
	void from_json(const json& j, solution_base<GAP_COUNT, MIN_GAP_VALUE, MAX_GAP_VALUE>& sol) {
        j.at("gaps").get_to(sol.gaps);
    }
}