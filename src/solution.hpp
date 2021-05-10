#pragma once
#include <array>
#include "json.hpp"
#include "array_utils.hpp"
#include "iterator_utils.hpp"

namespace ga_solution {

    using json = nlohmann::json;

    template <std::size_t GAP_COUNT, std::size_t MIN_GAP_VALUE, std::size_t MAX_GAP_VALUE>
	struct solution_base {
		std::array<int, GAP_COUNT> gaps;

        solution_base() {}

        solution_base(std::vector<int> x) {
            init();
            std::copy(x.begin(), x.end(), gaps.begin());
            normalize();
        }

		void init() {
			array_utils::fill_random(gaps, (int)MIN_GAP_VALUE, (int)MAX_GAP_VALUE);
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
			// copy first solutions gaps to new solution
			std::copy(gaps.begin(), gaps.end(), X_new.gaps.begin());
			// copy every other gap from second solution
			iterator_utils::copy_every_other_n(X2.gaps.begin(), X2.gaps.end(), X_new.gaps.begin(), 2);

			X_new.normalize();

			return X_new;
		}

		solution_base mutate(
		const std::function<double(void)> &rnd01,
		double shrink_scale) const {
			// TODO: determine need of mutating all of the gaps
			solution_base X_new = *this;
			for (int i = 0; i < gaps.size(); i++) {
				bool do_mutate = random_utils::get_random_bool();
				if (do_mutate) {
					int new_gap = random_utils::get_random_int((int)MIN_GAP_VALUE, (int)MAX_GAP_VALUE);
					X_new.gaps[i] = new_gap;
				}
			}
			X_new.normalize();

			return X_new;
		}
	};

    template <std::size_t GAP_COUNT, std::size_t MIN_GAP_VALUE, std::size_t MAX_GAP_VALUE>
    void to_json(json& j, const solution_base<GAP_COUNT, MIN_GAP_VALUE, MAX_GAP_VALUE>& sol) {
		j = json{
			{"gaps", sol.gaps},
		};
	}

    template <std::size_t GAP_COUNT, std::size_t MIN_GAP_VALUE, std::size_t MAX_GAP_VALUE>
	void from_json(const json& j, solution_base<GAP_COUNT, MIN_GAP_VALUE, MAX_GAP_VALUE>& sol) {
        j.at("gaps").get_to(sol.gaps);
    }
}